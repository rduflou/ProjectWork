// Compile with: g++ -O3 -o test project.cpp -lCGAL -lgmp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <utility>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/lloyd_optimize_mesh_2.h>

int main() {

	// Read in
	std::cout << "READING IN FILE" << std::endl;

	std::ifstream inputf;
	inputf.open("pear.txt");
    	inputf.unsetf(std::ios_base::skipws);	
	int N = std::count(std::istream_iterator<char>(inputf), std::istream_iterator<char>(), '\n');
	int n = 20;
	std::cout << "Number of lines in file: " << N << std::endl;
	inputf.close();
	inputf.open("pear.txt");

	std::string line;
	double x_edge [n], y_edge [n];

	for (int i=0; i<N ;++i) {
		std::getline( inputf, line, '\n' );
		if ((((i*n) % N) < n) || (i == N-1)){
			std::stringstream ss( line );
			if (!(ss >> x_edge[i*n/N]) || !(ss >> y_edge[i*n/N])) {
				std::cout << "Input file contains invalid data..." << std::endl;
				return 1;
			}		}
	}
	std::cout << "Number of edge points introduced: " << n << std::endl;
	inputf.close();

	// Building mesh
	std::cout << "BUILDING MESH" << std::endl;
	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
	typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
	typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
	typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
	typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
	typedef CGAL::Delaunay_mesh_size_criteria_2<CDT> Criteria;
	typedef CDT::Vertex_handle Vertex_handle;
	typedef CDT::Point Point;

  	CDT cdt;
	Vertex_handle first = cdt.insert(Point(x_edge[0],y_edge[0]));
	Vertex_handle prev = first;
	Vertex_handle next;
	for (int i=1;i<n;++i) {
		next = cdt.insert(Point(x_edge[i],y_edge[i]));
		cdt.insert_constraint(prev,next);
		prev = next;
	}
	cdt.insert_constraint(next,first);

	std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
	std::cout << "Meshing the triangulation..." << std::endl;
	CGAL::refine_Delaunay_mesh_2(cdt, Criteria(0.125, 10));
	std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;

	//std::cout << "Lloyd optimization..." << std::endl;	
  	//CGAL::lloyd_optimize_mesh_2(cdt,CGAL::parameters::max_iteration_number = 10);	
	//std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;

	CDT::Finite_vertices_iterator vit;
	int i = 0;
	for (vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) {
		vit->info() = i;
		i++;
	}	

	// Converting to self-made data structure
	std::cout << "CONVERTING MESH TO DATA CLASSES" << std::endl;
	CDT::Finite_faces_iterator fit;
	int i1, i2, i3;
	double x0, y0, x1, y1, x2, y2;
	bool edge_0, edge_1, edge_2;
	for (fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) {
		i1=fit->vertex(0)->info();
		x0=fit->vertex(0)->point().x();
		y0=fit->vertex(0)->point().y();
		i1=fit->vertex(1)->info();
		x1=fit->vertex(1)->point().x();
		y1=fit->vertex(1)->point().y();
		i1=fit->vertex(2)->info();
		x2=fit->vertex(2)->point().x();
		y2=fit->vertex(2)->point().y();
		edge_0 = false;
		edge_1 = false;
		edge_2 = false;
		if (cdt.is_infinite(fit->neighbor(0))){
			edge_1 = true;
			edge_2 = true;
		}
		if (cdt.is_infinite(fit->neighbor(1))){
			edge_2 = true;
			edge_0 = true;
		}
		if (cdt.is_infinite(fit->neighbor(2))){
			edge_0 = true;
			edge_1 = true;
		}
		// Do ya thing here pal!
	}

	// Building the matrix
	std::cout << "BUILDING FEM MATRIX (empty)" << std::endl;

	// Solving the matrix
	std::cout << "SOLVING FEM MATRIX (empty)" << std::endl;

	
	return 0;
}
