#include <gmsh.h>
#include <set>

int main(int argc, char **argv) {
    gmsh::initialize();
    gmsh::model::add("torus");
    
   gmsh::model::occ::addTorus(0, 0, 0, 5, 3, 1);
   gmsh::model::occ::addTorus(0, 0, 0, 5, 2.3, 2);

  std::vector<std::pair<int, int> > ov;
  std::vector<std::vector<std::pair<int, int> > > ovv;
  gmsh::model::occ::cut({{3, 1}}, {{3, 2}}, ov, ovv, 3);

  gmsh::option::setNumber("Mesh.MeshSizeMin",0.7);
    gmsh::option::setNumber("Mesh.MeshSizeMax",1.0);


    gmsh::model::occ::synchronize();
    
    gmsh::model::mesh::generate(3);
    
    gmsh::write("torus.msh");
    
    // Показываем окно (если не указан -nopopup)
    std::set<std::string> args(argv, argv + argc);
    if(!args.count("-nopopup")) gmsh::fltk::run();
    
    gmsh::finalize();
    return 0;
}
