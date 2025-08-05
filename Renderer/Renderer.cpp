#include "Renderer.hpp"

#include <iostream>
#include <syncstream>
#include <math.h>
#include <random>

Renderer::Renderer(int w, int h, float fov, float fr, bool alpha)
: worker_count(std::max(1, (int)std::thread::hardware_concurrency()-1)),
width(w), height(h), FOV(fov), framerate_setpoint(fr), delay_setpoint((long) (1e6/fr)), alpha_blending(alpha),
sync_point(worker_count, [&] { /*std::cout << "Completed" << std::endl;*/ })
{
	FOV_factor = 2 / (width * tan((M_PI-FOV)/2));
	initialize_image();
}

Renderer::~Renderer()
{
	free_image();
}

std::chrono::microseconds Renderer::get_time_to_render() const
{
	return time_to_render;
}
std::chrono::microseconds Renderer::get_delay_setpoint() const
{
	return delay_setpoint;
}
void Renderer::set_delay_setpoint(std::chrono::microseconds delay)
{
	delay_setpoint = delay;
	framerate_setpoint = 1e6/delay.count();
}
void Renderer::set_framerate_setpoint(float fr)
{
	framerate_setpoint = fr;

	delay_setpoint = std::chrono::microseconds((long) (1e6/fr));
}
std::chrono::microseconds Renderer::get_actual_delay() const
{
	return actual_delay;
}
std::chrono::microseconds Renderer::get_time_to_wait() const
{
	return time_to_wait;
}
std::chrono::microseconds Renderer::get_time_to_segment() const
{
	return time_to_segment;
}


int Renderer::get_width() const
{
	return width;
}
void Renderer::set_width(int w)
{
	width = w;

	FOV_factor = 2 / (width * tan((M_PI-FOV)/2));
}

int Renderer::get_height() const
{
	return height;
}
void Renderer::set_height(int h)
{
	height = h;
}

float Renderer::get_fov() const
{
	return FOV;
}
void Renderer::set_fov(float fov)
{
	FOV = fov;
	FOV_factor = 2 / (width * tan((M_PI-FOV)/2));
}


void Renderer::translate_point(Vector& p)
{
	p -= visor_pos;
	
	p.rotate_y(visor->sin_azimuth, visor->cos_azimuth);
	p.rotate_x(visor->sin_altitude, visor->cos_altitude);
	//p.rotate_azimuth(visor->sin_azimuth, visor->cos_azimuth);
	//p.rotate_altitude(visor->sin_altitude, visor->cos_altitude);
}
void Renderer::project_point(Vector& p)
{
	/*
	float r = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
	p.x /= r * FOV_factor;
	p.y /= r * FOV_factor;
	//*/
	//*
	p.x /= p.z * FOV_factor;
	p.y /= p.z * FOV_factor;
	//*/

	p.x += width/2;
	p.y = height/2 - p.y - 1;
}

/*
void Renderer::adjust_point(Vector& p)
{
	p.x += width/2;

	p.y = height/2 - p.y - 1;
}*/

void Renderer::set_point(int x, int y, float z, Color color)
{

	
	//std::cout << "Before: " << y << std::endl;
	//y = height - y - 1;

	//std::cout << "Then:   " << y << std::endl;

	if(alpha_blending)
	{
		if(current_image[y][x].z < 0)
		{
			current_image[y][x].color = color * (color.a/255.0f);
			current_image[y][x].z = z;
		}
		else if(z < current_image[y][x].z)
		{
			current_image[y][x].color.alpha_blend_foreground(color);
			current_image[y][x].z = z;
		}
		else
		{
			color.alpha_blend_foreground(current_image[y][x].color);
			current_image[y][x].color = color;
		}
	}
	else
	{
		current_image[y][x] = {color, z};
	}
	
	render_pixel(x, y, current_image[y][x].color.hex());
}

void Renderer::draw_spatial_line(Vector a, Vector b, Color color)
{
	translate_point(a);
	translate_point(b);
	project_point(a);
	project_point(b);
	
	if(a.z < 50 && b.z < 50) return;
	
	// For readability sake
	float x1 = a.x, y1 = a.y, z1 = a.z;
	float x2 = b.x, y2 = b.y, z2 = b.z;
	
	// Out-of-bound lines, that span only outside of field of view
	if(x1 < 0 && x2 < 0) return;
	if(y1 < 0 && y2 < 0) return;
	if(x1 > width && x2 > width) return;
	if(y1 > height && y2 > height) return;
		
	// The slope of the line
	float dydx_slope = (y2 - y1) / (x2 - x1);
	float dzdx_slope = (z2 - z1) / (x2 - x1);

	// Where the line's x begins, where it ends, and the height (y coordinate) and depth(z coordinate) on its beginning
	int begin, end, origin_x, origin_y, origin_z;
	
	if(dydx_slope <= 1 && dydx_slope >= -1) {
		if(x1 < x2) {
			begin = x1;
			end = x2;
			origin_y = y1;
			origin_z = z1;
		} else {
			begin = x2;
			end = x1;
			origin_y = y2;
			origin_z = z2;
		}

		for(int x = std::max(begin, 0); x < std::min(end, width-1); ++x) {
			// Skips out-of-bound points
			
			int y = (x - begin) * dydx_slope + origin_y;
			int z = (x - begin) * dzdx_slope + origin_z;
			
			if(y < 0) continue;
			if(y >= height) break;
			if(z <= 0) continue;

			if(current_image[y][x].z > z || current_image[y][x].z <= 0 || alpha_blending) {
				float luminosity = std::min(1.0, 200000/pow(z, 2));
				
				//set_point(x, y, color.hex() * luminosity); // funky total!
				set_point(x, y, z, color * luminosity);
			}
		}
	}
	// Line is too steep taking Y as a function of X: we therefore take X as a function of Y
	else {
		float dxdy_slope = (x2 - x1) / (y2 - y1);
		float dzdy_slope = (z2 - z1) / (y2 - y1);
		if(y1 < y2) {
			begin = y1;
			end = y2;
			origin_x = x1;
			origin_z = z1;
		} else {
			begin = y2;
			end = y1;
			origin_x = x2;
			origin_z = z2;
		}
		
		for(int y = std::max(begin, 0); y < std::min(end, height-1); ++y) {
			int x = (y - begin) * dxdy_slope + origin_x;
			int z = (y - begin) * dzdy_slope + origin_z;
			
			if(x < 0) continue;
			if(x >= width) break;
			if(z <= 0) continue;
			
			if(current_image[y][x].z > z || current_image[y][x].z <= 0 || alpha_blending) {
				float luminosity = std::min(1.0, 200000/pow(z, 2));

				//set_point(x, y, color.hex() * luminosity); // funky total!
				set_point(x, y, z, color * luminosity);
			}
		}
	}
}




void Renderer::draw_spatial_triangle_experimental(const Triangle& tri, bool outline)
{
	Vector a = tri.a, b = tri.b, c = tri.c;
	
	
	translate_point(a);
	translate_point(b);
	translate_point(c);
	project_point(a);
	project_point(b);
	project_point(c);


	// Finds a vector perpendicular to the given triangle in order to determine how shiny it should be
	Vector normal = (a - b).cross(a - c);
	normal /= normal.modulus();
	
	float cos_alpha = fabs(normal.dot(Vector(0, 0, 1)));
	
	
	if(a.z < 0 || b.z < 50 || c.z < 50) return;
	
	if(a.x > width && b.x > width && c.x > width) return;
	if(a.x < 0 && b.x < 0 && c.x < 0) return;
	if(a.y > height && b.y > height && c.y > height) return;
	if(a.y < 0 && b.y < 0 && c.y < 0) return;


	Vector top, middle, bottom;
	
	// Find the vertical position of the three points with respect to each other: top, middle or bottom
	if(a.y <= b.y && a.y <= c.y) { // a is the topmost point
		top = a;
		if(b.y >= c.y) { // b is the bottommost point (and c is in the middle)
			bottom = b;
			middle = c;
		} else { // c is the bottommost point (and b is in the middle)
			bottom = c;
			middle = b;
		}
	}
	else if(b.y <= a.y && b.y <= c.y) { // b is the topmost point
		top = b;
		if(c.y >= a.y) { // c is the bottommost point (and a is int the middle)
			bottom = c;
			middle = a;
		} else { // a is the bottommost point (and c is int the middle)
			bottom = a;
			middle = c;
		}
	}
	else { // c is the topmost point
		top = c;
		if(a.y >= b.y) { // a is the bottommost point (and b is int the middle)
			bottom = a;
			middle = b;
		} else { // b is the bottommost point (and a is int the middle)
			bottom = b;
			middle = a;
		}
	}
	
	float dxdy_topmiddle =		(top.x-middle.x)	/	(top.y-middle.y);
	float dxdy_topbottom =		(top.x-bottom.x)	/	(top.y-bottom.y);
	float dxdy_middlebottom =	(middle.x-bottom.x)	/	(middle.y-bottom.y);
	float dzdy_topmiddle =		(top.z-middle.z)	/	(top.y-middle.y);
	float dzdy_topbottom =		(top.z-bottom.z)	/	(top.y-bottom.y);
	float dzdy_middlebottom =	(middle.z-bottom.z)	/	(middle.y-bottom.y);
	
	int begin_x, begin_z, end_x, end_z, dzdx;
	float z;
	int begin_y = top.y < 0 ? 0 : top.y+1;
	int end_y = bottom.y >= height ? height-1 : bottom.y;

	if(end_y < 0 || begin_y > height) return;
	
	
	for(int y = begin_y; y <= end_y; ++y) {
		if(y < middle.y) {
			begin_x = dxdy_topmiddle * (y-top.y) + top.x;
			begin_z = dzdy_topmiddle * (y-top.y) + top.z;
		} else {
			begin_x = dxdy_middlebottom * (y-middle.y) + middle.x;
			begin_z = dzdy_middlebottom * (y-middle.y) + middle.z;
		}
	
		end_x = dxdy_topbottom * (y-top.y) + top.x;
		end_z = dzdy_topbottom * (y-top.y) + top.z;

		if(end_x < begin_x) {
			std::swap(begin_x, end_x);
			std::swap(begin_z, end_z);
		}

		if(end_x == begin_x) end_x++;
		
		dzdx = (end_z-begin_z)/(end_x-begin_x);
		
		if(end_x >= width) end_x = width-1;
		if(begin_x < 0)	begin_x = 0;

		for(int x = begin_x; x <= end_x; ++x) {
			z = dzdx * (x-begin_x) + begin_z;
			
			if((current_image[y][x].z > z || current_image[y][x].z <= 0 || alpha_blending) && z > 0) {
				float luminosity = std::min(1.0, 200000/pow(z, 2));
				
				set_point(x, y, z, tri.color * ((cos_alpha + luminosity)/2));
			}
		}
	}
}



void Renderer::draw_spatial_triangle(const Triangle& tri, bool outline, bool skiptrans)
{
	Vector a = tri.a, b = tri.b, c = tri.c;
	
	if(outline)
	{
		draw_spatial_line(a, b, Color::teal);
		draw_spatial_line(b, c, Color::teal);
		draw_spatial_line(c, a, Color::teal);
	}

	if(!skiptrans)
	{
		translate_point(a);
		translate_point(b);
		translate_point(c);
	}

	//if(a.z < 1 || b.z < 1 || c.z < 1) return;

	//*
	Vector near, mid, far;
	if(a.z <= b.z && a.z <= c.z) { // a is the nearest point
		near = a;
		if(b.z >= c.z) { // b is the farthest point (and c is in the middle)
			far = b;
			mid = c;
		} else { // c is the farthest point (and b is in the middle)
			far = c;
			mid = b;
		}
	}
	else if(b.z <= a.z && b.z <= c.z) { // b is the nearest point
		near = b;
		if(c.z >= a.z) { // c is the farthest point (and a is int the middle)
			far = c;
			mid = a;
		} else { // a is the farthest point (and c is int the middle)
			far = a;
			mid = c;
		}
	}
	else { // c is the nearest point
		near = c;
		if(a.z >= b.z) { // a is the farthest point (and b is int the middle)
			far = a;
			mid = b;
		} else { // b is the farthest point (and a is int the middle)
			far = b;
			mid = a;
		}
	}

	// Triangle is behind visor
	if(near.z < 1 && mid.z < 1 && far.z < 1) return;

	if(near.z < 1 && mid.z < 1)
	{
		Vector nearfar = far - near;
		nearfar *= ((1-near.z)/nearfar.z);

		Vector midfar = far - mid;
		midfar *= ((1-mid.z)/midfar.z);

		//printf("nearfar = (%.6g, %.6g, %.6g)\n", (near+nearfar).x, (near+nearfar).y, (near+nearfar).z);
		//printf("midfar = (%.6g, %.6g, %.6g)\n", (mid+midfar).x, (mid+midfar).y, (mid+midfar).z);

		draw_spatial_triangle(Triangle(near + nearfar, mid + midfar, far, tri.color), outline, true);
		return;
	}
	else if(near.z < 1)
	{
		Vector nearfar = far - near;
		nearfar *= ((1-near.z)/nearfar.z);

		Vector nearmid = mid - near;
		nearmid *= ((1-near.z)/nearmid.z);

		//printf("nearfar = (%.6g, %.6g, %.6g)\n", (near+nearfar).x, (near+nearfar).y, (near+nearfar).z);

		draw_spatial_triangle(Triangle(near + nearfar, mid, far, tri.color), outline, true);
		draw_spatial_triangle(Triangle(near + nearfar, near + nearmid, mid, tri.color), outline, true);

		return;
	}


	//*/

	// Finds a vector perpendicular to the given triangle in order to determine how shiny it should be
	Vector orthogonal = (a - b).cross(a - c);
	//if(orthogonal.modulus() < 2) return;
	Vector orthonormal = orthogonal.normalise();
	
	// Finds the z = Ax + By + C expression for the plane containing this triangle
	// Used later to determine the z-coordinate of the perspectivised triangle
	// (By perspectivise I mean multiply the x,y of its points by 1/z and use some slick maths tricks)
	double det = a.x * (b.y-c.y) + a.y * (c.x-b.x) + b.x*c.y - c.x*b.y;
	if(det == 0) return;
	double A = (a.z * (b.y-c.y) + a.y * (c.z-b.z) + b.z*c.y - c.z*b.y)/det;
	double B = (a.x * (b.z-c.z) + a.z * (c.x-b.x) + b.x*c.z - c.x*b.z)/det;
	//float C = (a.x * (b.y*c.z - c.y*b.z) + a.y * (c.x*b.z - b.x*c.z) + a.z * (b.x*c.y - c.x*b.y))/det;
	double C = a.z - A*a.x - B*a.y;


	project_point(a);
	project_point(b);
	project_point(c);


	float cos_alpha = orthonormal.dot(Vector(0, 0, 1));
	cos_alpha *= cos_alpha;

	
	// Triangle is outside FOV
	if(a.x > width && b.x > width && c.x > width) return;
	if(a.x < 0 && b.x < 0 && c.x < 0) return;
	if(a.y > height && b.y > height && c.y > height) return;
	if(a.y < 0 && b.y < 0 && c.y < 0) return;

	Vector top, middle, bottom;
	
	// Find the vertical position of the three points with respect to each other: top, middle or bottom
	if(a.y <= b.y && a.y <= c.y) { // a is the topmost point
		top = a;
		if(b.y >= c.y) { // b is the bottommost point (and c is in the middle)
			bottom = b;
			middle = c;
		} else { // c is the bottommost point (and b is in the middle)
			bottom = c;
			middle = b;
		}
	}
	else if(b.y <= a.y && b.y <= c.y) { // b is the topmost point
		top = b;
		if(c.y >= a.y) { // c is the bottommost point (and a is int the middle)
			bottom = c;
			middle = a;
		} else { // a is the bottommost point (and c is int the middle)
			bottom = a;
			middle = c;
		}
	}
	else { // c is the topmost point
		top = c;
		if(a.y >= b.y) { // a is the bottommost point (and b is int the middle)
			bottom = a;
			middle = b;
		} else { // b is the bottommost point (and a is int the middle)
			bottom = b;
			middle = a;
		}
	}
	
	

	double dxdy_topmiddle =		(top.x-middle.x)	/	(top.y-middle.y);
	double dxdy_topbottom =		(top.x-bottom.x)	/	(top.y-bottom.y);
	double dxdy_middlebottom =	(middle.x-bottom.x)	/	(middle.y-bottom.y);
	

	int begin_x, end_x;
	int begin_y = top.y < 0 ? 0 : top.y+1;
	int end_y = bottom.y >= height ? height-1 : bottom.y;

	if(end_y < 0 || begin_y > height) return;

	++rendered_triangle_count;

	for(int y = begin_y; y <= end_y; ++y)
	{
		if(y < middle.y)
		{
			begin_x = dxdy_topmiddle * (y-top.y) + top.x;
		}
		else
		{
			if(middle.y == bottom.y) break;
			begin_x = dxdy_middlebottom * (y-middle.y) + middle.x;
		}
	
		end_x = dxdy_topbottom * (y-top.y) + top.x;

		if(end_x < begin_x)
		{
			std::swap(begin_x, end_x);
		}

		
		if(end_x >= width) end_x = width;
		if(begin_x < 0)	begin_x = 0;
		
		for(int x = begin_x; x < end_x; ++x)
		{
			

			int X = x - width/2;
			int Y = height/2 - y - 1;


			float z = C * (1 + 1/(1/( FOV_factor * (A*X + B*Y) ) - 1) );

			//printf("(%.6g, %.6g, %.6g, %.6g)\n", x, y, z, z_);
			//std::cout << "X = " << x << ", Y = " << y << ", Z = " << z << ", Z_ = " << z_ << std::endl;

			if(z > 0 && (current_image[y][x].z > z || current_image[y][x].z <= 0 || alpha_blending))
			{
				//float luminosity = ;
				
				set_point(x, y, z, tri.color * ((cos_alpha + std::min(1.0f, 800000/(z*z)))/2));
			}
		}
	}
}


void Renderer::initialize_image()
{
	current_image = (Projected_point**)calloc(height, sizeof(Projected_point*));
	for(int y = 0; y < height; ++y)
	{
		current_image[y] = (Projected_point*)calloc(width, sizeof(Projected_point));
		
	}
}

void Renderer::wipe_image()
{
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			current_image[y][x].z = -1;
		}
	}
}
void Renderer::free_image()
{
	for(int y = 0; y < height; ++y)
	{
		free(current_image[y]);
	}
	free(current_image);
}





void Renderer::fetch_triangles()
{
	//std::cout << "Segmenting..." << std::endl;

	auto start = std::chrono::high_resolution_clock::now();

	triangle_chunks.clear();
	for(int i = 0; i < worker_count; ++i)
	{
		triangle_chunks.push_back(std::vector<Triangle>{});

		if(last_chunk_size > 0)
			triangle_chunks[i].reserve(last_chunk_size);
	}
	int count = 0;

	for(const Shape* shape : visor->get_simulation()->get_shapes())
	{
		// so that the shape doesn't get split if it moves while drawing it
		Vector pos = shape->pos;

		for(const Triangle& tri : shape->mesh)
		{
			triangle_chunks[count].push_back(tri + pos);

			count++;
			if(count == worker_count) count = 0;

			total_triangle_count++;
		}
	}

	last_chunk_size = triangle_chunks.front().size();

	time_to_segment = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);

	//std::cout << "It took me " << time_to_segment.count()/1e6 << " seconds to segment." << std::endl;
}

void Renderer::work(int triangle_chunk_index)
{
	//std::osyncstream(std::cout) << "Worker " << triangle_chunk_index << " performing iteration" << std::endl;

	// we put the first thread in charge of wiping the image
	// and resetting the counters
	// and fetching the triangles
	if(triangle_chunk_index == 0)
	{
		pre_render();

		wipe_image();

		fetch_triangles();
		//std::cout << "Chunk: " << triangle_chunk_index << "; Size: " << triangle_chunks.size() << std::endl;

		rendered_triangle_count = 0;
		
		visor_pos = visor->pos;

		//draw_spatial_triangle(Triangle(Vector(-100, 0, 0), Vector(100, 0, 0), Vector(0, 100, 0), Color::white));
	}

	sync_point.arrive_and_wait();

	//std::osyncstream(std::cout) << "(662) Chunk: " << triangle_chunk_index << "; Size: " << triangle_chunks.size() << std::endl;


	//int n = 0;

	if(triangle_chunk_index < triangle_chunks.size())
	{
		for(const Triangle& tri : triangle_chunks[triangle_chunk_index])
		{
			draw_spatial_triangle(tri);
		}
		//std::osyncstream(std::cout) << "Rendered " << triangle_chunks[triangle_chunk_index].size() << "/" << total_triangle_count << " triangles" << std::endl;
	}


	sync_point.arrive_and_wait();


	// 
	if(triangle_chunk_index == 0)
	{
		for(const Shape* shape : visor->get_simulation()->get_shapes())
		{
			if(shape->trail.size() > 2)
			{
				int counter = 0;
				for(auto it = shape->trail.begin(); it != shape->trail.end()-1;)
				{
					Vector a = *(it);
					Vector b = *(++it);

					Color col = shape->color;
					col.a = 255;
					col = col * std::max(0.2f, (counter++)/(float)shape->trail.size());

					draw_spatial_line(a, b, col);
				}
			}
		}


		post_render();
	}
}

void Renderer::loop(int triangle_chunk_index)
{
	//std::osyncstream(std::cout) << "Starting worker " << triangle_chunk_index << std::endl;

	while(run)
	{
		auto start = std::chrono::high_resolution_clock::now();
		
		work(triangle_chunk_index);

		sync_point.arrive_and_wait();

		// After all threads are done rendering, wait an extra bit if needed
		if(triangle_chunk_index == 0)
		{
			time_to_render = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
		
			time_to_wait = time_to_render > delay_setpoint ? std::chrono::microseconds(0) : delay_setpoint - time_to_render;

			actual_delay = time_to_render + time_to_wait;

			std::this_thread::sleep_for(time_to_wait);
		}


		sync_point.arrive_and_wait();
	}
}
void Renderer::start()
{
	workers.reserve(worker_count);

	for(int i = 0; i < worker_count; ++i)
	{
		workers.emplace_back(&Renderer::loop, this, i);
		workers[i].detach();
	}
}
void Renderer::end()
{
	run = false;
}

void Renderer::set_visor(Visor* v)
{
	visor = v;
}

Visor* Renderer::get_visor() const
{
	return visor;
}
void Renderer::remove_visor()
{
	visor = nullptr;
}