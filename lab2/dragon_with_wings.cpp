#include <iostream>
#include <cmath>
#include <vector>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkTetra.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>

#include <gmsh.h>

using namespace std;

// Класс расчётной точки
class CalcNode
{
// Класс сетки будет friend-ом точки
friend class CalcMesh;

protected:
    // Координаты
    double x;
    double y;
    double z;
    // Некая величина, в попугаях
    double smth;
    // Скорость
    double vx;
    double vy;
    double vz;

public:
    // Конструктор по умолчанию
    CalcNode() : x(0.0), y(0.0), z(0.0), smth(0.0), vx(0.0), vy(0.0), vz(0.0)
    {
    }

    // Конструктор с указанием всех параметров
    CalcNode(double x, double y, double z, double smth) 
            : x(x), y(y), z(z), smth(smth)
    {
    }

    // Метод отвечает за перемещение точки
    // Движемся время tau из текущего положения с текущей скоростью
void move(double tau, int Steps)
{
    
        y = y+6*tau;
    
    
}
    void move_right(double tau, int Steps) 
    {
        double new_x, new_y, new_z;  
        double const x_0 = 37.5;
        double const y_0 = 45;
        x = x-x_0;
        y = y-y_0;
        double omega = 7;
        double phi = omega*tau;
        double angle = omega*tau*Steps;

        if(angle <=  M_PI/6)
        {
            new_x = x*cos(phi) + y*sin(phi);
            new_y = -x*sin(phi) + y*cos(phi); 
            new_z = z;
        }
        else //не первый шаг, теперь детектим поворот на 60 градусов
        {
            int i = int((angle+M_PI/6)/(M_PI/3));
            if(i%2 ==0){        
                new_x = x*cos(phi) + y*sin(phi);
                new_y = -x*sin(phi) + y*cos(phi); 
                new_z = z;
            }
            else
            {
                new_x = x*cos(phi) - y*sin(phi);
                new_y = x*sin(phi) + y*cos(phi);
                new_z = z;
            }
        }
            x = new_x + x_0;
            y = new_y + y_0;
            z = new_z;
        }
        

    void move_left(double tau, int Steps) 
    {
        double new_x, new_y, new_z;  
        double const x_0 = 61.7;
        double const y_0 = 45;
        x = x-x_0;
        y = y-y_0;
        double omega = 7;
        double phi = omega*tau;
        double angle = omega*tau*Steps;

        if(angle <=  M_PI/6)
        {
            new_x = x*cos(phi) - y*sin(phi);
            new_y = x*sin(phi) + y*cos(phi); 
            new_z = z;
        }
        else //не первый шаг, теперь детектим поворот на 60 градусов
        {
            int i = int((angle+M_PI/6)/(M_PI/3));
            if(i%2 ==0){        
                new_x = x*cos(phi) - y*sin(phi);
                new_y = x*sin(phi) + y*cos(phi); 
                new_z = z;
            }
            else
            {
                new_x = x*cos(phi) +y*sin(phi);
                new_y = -x*sin(phi) + y*cos(phi);
                new_z = z;
            }
        }
            x = new_x + x_0;
            y = new_y + y_0;
            z = new_z; 
    }  
    };

        // Класс элемента сетки
        class Element
        {
        // Класс сетки будет friend-ом и элемента тоже
        // (и вообще будет нагло считать его просто структурой)
        friend class CalcMesh;

        protected:
            // Индексы узлов, образующих этот элемент сетки
            unsigned long nodesIds[4];
        };

// Класс расчётной сетки
class CalcMesh
{
protected:
    // 3D-сетка из расчётных точек
    vector<CalcNode> nodes;
    vector<CalcNode> nodes_wings;
    //vector<CalcNode> nodes_else;
    vector<size_t> first_wing_indices;
    vector<size_t> second_wing_indices;
    vector<Element> elements;

public:

    // Конструктор сетки из заданного stl-файла
    CalcMesh(const std::vector<double>& nodesCoords, const std::vector<std::size_t>& tetrsPoints) {
    vector<CalcNode> nodes_wings_temp;
    //vector<CalcNode> nodes_else_temp;
        // Пройдём по узлам в модели gmsh
        nodes.resize(nodesCoords.size() / 3);
        for(unsigned int i = 0; i < nodesCoords.size() / 3; i++) {
            // Координаты заберём из gmsh
            double pointX = nodesCoords[i*3];
            double pointY = nodesCoords[i*3 + 1];
            double pointZ = nodesCoords[i*3 + 2];
            // Модельная скалярная величина распределена как-то вот так
            double smth = 0;
            
            CalcNode node(pointX, pointY, pointZ, smth);
            nodes[i] = node;
            
            if((pointX >= 0 && pointZ >= 0.3*pointX+11.6 && pointX <= 37.5 && 
                pointZ <= 0.87*pointX + 60.72 && pointZ <= 40 && pointY >= 40 && pointY <= 60)
                || (pointZ >= 40 && pointZ >= 2.7*pointX - 26.36 && pointZ <= 47.12 && 
                    pointX >= 0 && pointY >= 40 && pointY <= 60)) 
            {         
                first_wing_indices.push_back(i);
            }

           if(
    (
        pointZ <= 40.0 &&                              // Ограничение по высоте
        pointZ >= 0.3*(99.2 - pointX) + 11.6 &&        // Нижняя граница (A-B)
        pointZ <= -0.87*(99.2 - pointX) + 60.8 &&      // Верхняя граница (C-D)
        pointX >= 61.7 &&                               // Левая граница по X (B-C)
        pointX <= 160 && pointY >= 40 && pointY <= 60                                // Правая граница по X (D)
    )
    ||
    (
        pointZ >= 40.0 &&                               // Выше или равно 40
        pointZ >= 2.7*(99.2 - pointX) - 26.36 &&        // Нижняя граница (D-E)
        pointZ <= 47.12 &&                               // Верхняя граница (E-F)
        pointX >= 72.78 &&                               // Левая граница (E)
        pointX <= 160  && pointY >= 40 && pointY <= 60                                 // Правая граница (F)
    )
           )
        {
            second_wing_indices.push_back(i);
        }

          
        }
            //nodes_wings = nodes_wings_temp;
            //nodes_else = nodes_else_temp;
        // Пройдём по элементам в модели gmsh
        elements.resize(tetrsPoints.size() / 4);
        for(unsigned int i = 0; i < tetrsPoints.size() / 4; i++) {
            elements[i].nodesIds[0] = tetrsPoints[i*4] - 1;
            elements[i].nodesIds[1] = tetrsPoints[i*4 + 1] - 1;
            elements[i].nodesIds[2] = tetrsPoints[i*4 + 2] - 1;
            elements[i].nodesIds[3] = tetrsPoints[i*4 + 3] - 1;
        }
    }

    // Метод отвечает за выполнение для всей сетки шага по времени величиной tau
    void doTimeStep(double tau, int step) {
        // for(int i =0; i < nodes.size(); ++i) {
        //     nodes[i].move(tau,step);
        //  }
        // Двигаем только точки крыльев по индексам
        for(size_t idx : first_wing_indices) {
            nodes[idx].move_right(tau, step);
        }
         for(size_t idx : second_wing_indices) {
            nodes[idx].move_left(tau,step);
         }
         
    }

    // Метод отвечает за запись текущего состояния сетки в снапшот в формате VTK
    void snapshot(unsigned int snap_number) {
        // Сетка в терминах VTK
        vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        // Точки сетки в терминах VTK
        vtkSmartPointer<vtkPoints> dumpPoints = vtkSmartPointer<vtkPoints>::New();

        // Скалярное поле на точках сетки
        auto smth = vtkSmartPointer<vtkDoubleArray>::New();
        smth->SetName("smth");

        // Векторное поле на точках сетки
        auto vel = vtkSmartPointer<vtkDoubleArray>::New();
        vel->SetName("velocity");
        vel->SetNumberOfComponents(3);

        // Обходим все точки нашей расчётной сетки
        for(unsigned int i = 0; i < nodes.size(); i++) {
            // Вставляем новую точку в сетку VTK-снапшота
            dumpPoints->InsertNextPoint(nodes[i].x, nodes[i].y, nodes[i].z);

            // Добавляем значение векторного поля в этой точке
            double _vel[3] = {nodes[i].vx, nodes[i].vy, nodes[i].vz};
            vel->InsertNextTuple(_vel);

            // И значение скалярного поля тоже
            smth->InsertNextValue(nodes[i].smth);
        }

        // Грузим точки в сетку
        unstructuredGrid->SetPoints(dumpPoints);

        // Присоединяем векторное и скалярное поля к точкам
        unstructuredGrid->GetPointData()->AddArray(vel);
        unstructuredGrid->GetPointData()->AddArray(smth);

        // А теперь пишем, как наши точки объединены в тетраэдры
        for(unsigned int i = 0; i < elements.size(); i++) {
            auto tetra = vtkSmartPointer<vtkTetra>::New();
            tetra->GetPointIds()->SetId( 0, elements[i].nodesIds[0] );
            tetra->GetPointIds()->SetId( 1, elements[i].nodesIds[1] );
            tetra->GetPointIds()->SetId( 2, elements[i].nodesIds[2] );
            tetra->GetPointIds()->SetId( 3, elements[i].nodesIds[3] );
            unstructuredGrid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
        }

        // Создаём снапшот в файле с заданным именем
        string fileName = "dragon_with_wings_with_wings-step-" + std::to_string(snap_number) + ".vtu";
        vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(unstructuredGrid);
        writer->Write();
    }
};

int main()
{
    // Шаг по времени
    double tau = 0.01;
    int numSteps = 100;

    const unsigned int GMSH_TETR_CODE = 4;

    // Теперь придётся немного упороться:
    // (а) построением сетки средствами gmsh,
    // (б) извлечением данных этой сетки в свой код.
    gmsh::initialize();
    gmsh::model::add("dragon_with_wings_with_wings");

    // Считаем STL
    try {
        gmsh::merge("dragon.stl"); 
    } catch(...) {
        gmsh::logger::write("Could not load STL mesh: bye!");
        gmsh::finalize();
        return -1;
    }

    // Восстановим геометрию
    double angle = 180;
    bool forceParametrizablePatches = true;
    bool includeBoundary = false;
    double curveAngle = 180;
    gmsh::model::mesh::classifySurfaces(angle * M_PI / 180., includeBoundary, 
                                        forceParametrizablePatches, curveAngle * M_PI / 180.);
    gmsh::model::mesh::createGeometry();

    // Зададим объём по считанной поверхности
    std::vector<std::pair<int, int> > s;
    gmsh::model::getEntities(s, 2);
    std::vector<int> sl;
    for(auto surf : s) sl.push_back(surf.second);
    int l = gmsh::model::geo::addSurfaceLoop(sl);
    gmsh::model::geo::addVolume({l});
    gmsh::model::geo::synchronize();

    // Зададим мелкость желаемой сетки
    int f = gmsh::model::mesh::field::add("MathEval");
    gmsh::model::mesh::field::setString(f, "F", "4");
    gmsh::model::mesh::field::setAsBackgroundMesh(f);

    // Построим сетку
    gmsh::model::mesh::generate(3);

    // Теперь извлечём из gmsh данные об узлах сетки
    std::vector<double> nodesCoord;
    std::vector<std::size_t> nodeTags;
    std::vector<double> parametricCoord;
    gmsh::model::mesh::getNodes(nodeTags, nodesCoord, parametricCoord);

    // И данные об элементах сетки тоже извлечём
    std::vector<std::size_t>* tetrsNodesTags = nullptr;
    std::vector<int> elementTypes;
    std::vector<std::vector<std::size_t>> elementTags;
    std::vector<std::vector<std::size_t>> elementNodeTags;
    gmsh::model::mesh::getElements(elementTypes, elementTags, elementNodeTags);
    
    for(unsigned int i = 0; i < elementTypes.size(); i++) {
        if(elementTypes[i] != GMSH_TETR_CODE)
            continue;
        tetrsNodesTags = &elementNodeTags[i];
    }

    if(tetrsNodesTags == nullptr) {
        cout << "Can not find tetra data. Exiting." << endl;
        gmsh::finalize();
        return -2;
    }

    cout << "The model has " << nodeTags.size() << " nodes and " 
         << tetrsNodesTags->size() / 4 << " tetrs." << endl;

    // Проверки
    for(int i = 0; i < nodeTags.size(); ++i) {
        assert(i == nodeTags[i] - 1);
    }
    assert(tetrsNodesTags->size() % 4 == 0);

    // ** ИСПРАВЛЕНИЕ 1: Создаем сетку ПОСЛЕ загрузки данных **
    CalcMesh mesh(nodesCoord, *tetrsNodesTags);

    // ** ИСПРАВЛЕНИЕ 2: Закрываем gmsh ПОСЛЕ создания mesh **
    gmsh::finalize();

    // ** ИСПРАВЛЕНИЕ 3: Сохраняем начальное состояние **
    cout << "Saving initial state..." << endl;
    mesh.snapshot(0);

    // ** ИСПРАВЛЕНИЕ 4: Запускаем временной цикл **
    cout << "Starting time evolution for " << numSteps << " steps..." << endl;
    
    for(int step = 1; step <= numSteps; step++) {
        mesh.doTimeStep(tau, step);
        mesh.snapshot(step);
        
        if(step % 10 == 0) {
            cout << "Step " << step << " completed" << endl;
        }
    }
    
    cout << "Simulation finished! Generated " << numSteps + 1 << " snapshots." << endl;

    return 0;
}
