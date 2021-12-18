#include <stdio.h>
#include <iostream>


//Definición de macros para independizarse del S.O

#ifdef _WIN32
#define CLEAR "cls"
#elif defined(unix)||defined(_unix_)||defined(__unix)||defined(_APPLE_)||defined(_MACH_)
#define CLEAR "clear"
#else
#error "S.O no soportado para la limpieza de pantalla"
#endif

#ifdef _WIN32
#define PAUSE "pause"
#elif defined(unix)||defined(_unix_)||defined(__unix)||defined(_APPLE_)||defined(_MACH_)
#define PAUSE "read -p 'ENTER para continuar...' var"
#else
#error "S.O no soportado para la pausa de la pantalla"
#endif

//Arboles para guardar los ID tanto de médicos como de pacientes
struct nodoArbolPaciente{
  long dato;
  nodoArbolPaciente *der;
  nodoArbolPaciente *izq;
};

struct nodoArbolMedico{
  long dato;
  nodoArbolMedico *der;
  nodoArbolMedico *izq;
};

//Prototipos
nodoArbolPaciente *arbolPaciente=NULL;
nodoArbolMedico *arbolMedico=NULL;

//Funciones del arbol binario
nodoArbolPaciente *crearNodoArbolPaciente(long);
nodoArbolMedico *crearNodoArbolMedico(long);

// funciones de insertar en el arbol
void insertarNodoArbolPaciente(nodoArbolPaciente *&, long);
void insertarNodoArbolMedico(nodoArbolMedico *&, long);


//El siguiente es el nodo predispuesto para nuestra cola
typedef struct nodoPacientes{
  int ID_paciente;
  char nombres_Paciente[30];
  char apellidos_Paciente[30];
  char correo_electronicoPaciente[50];
  char sexo_Paciente;
  long numero_celularPaciente;
  char estado_civilPaciente[30];

  int cont_Visitas;
  int turno;
  char fecha_Atencion[11];
  //Sección de registro de historia
  char diagnostico_Paciente[1024];
  char tratamiento_Paciente[1024];
  char fecha_Proxcontrol[11];
  struct nodoPacientes* sig;
} nodoPacientes;

typedef struct nodoMedicos{
  int ID_Medico;
  char nombres_Medico[30];
  char apellidos_Medico[30];
  char correo_electronicoMedico[50];
  char sexo_Medico;
  long numero_celularMedico;
  char estado_civilMedico[30];

  struct nodoMedicos* sig;
} nodoMedicos;

//Apuntadores al nodo primero y último de las dos colas que almacenan médicos y pacientes.
nodoPacientes* primeroPacientes=NULL;
nodoPacientes* ultimoPacientes=NULL;
nodoMedicos* primeroMedicos=NULL;
nodoMedicos* ultimoMedicos=NULL;

//Declaración de funciones
void menu();
void menuRegistrar();
void addPaciente();
void addMedico();
void searchPaciente(nodoArbolPaciente *, long);
void searchMedico(nodoArbolMedico *, long);
void comprobarExistenciaIDPaciente(long);
void comprobarExistenciaIDMedico(long);
void mostrarPacientes();
void registrarConsulta();
void buscarHistorialVisitas();

//variables globales de busqueda
int n=0;
int m=0;


using namespace std;

//Función principal
int main(){
  menu();

  return 0;
}

//Definción de funciones
void menu(){
  int opt=1;
  do{
    cout<<".:Bienvendio al sistema de registros médicos:."<<endl<<endl;
    while(opt<9 && opt>0){
      cout<<"[_Menú de opciones_]"<<endl;
      cout<<"1. Añadir registros de nuevos pacientes"<<endl;
      cout<<"2. Añadir registros de nuevos médicos"<<endl;
      cout<<"3. Buscar pacientes"<<endl;
      cout<<"4. Buscar médicos"<<endl;
      cout<<"5. Buscar el historial de visitas de los pacientes"<<endl;
      cout<<"6. Mostrar los pacientes registrados en el sistema"<<endl;
      cout<<"7. Registrar consulta"<<endl;
      cout<<"8. Salir "<<endl;
      cout<<">>";

      cin>>opt;
      cin.get();
      if(opt>8 || opt<1){
        cout<<"¡Opcion no válida!"<<endl;
        system(PAUSE);
        system(CLEAR);
        opt=1;
      }
      else{
        break;
      }
    }
    switch(opt){
      case 1:
        system(CLEAR);
        addPaciente();
        system(CLEAR);
        break;
      case 2:
        system(CLEAR);
        addMedico();
        system(CLEAR);
        break;
      case 3:
        system(CLEAR);
        long tmp;
        cout<<"Menú de búsqueda empleando 'tree binary search method'"<<endl<<endl;
        cout<<"Por favor, digite su ID de Paciente: ";
        cin>>tmp;
        searchPaciente(arbolPaciente,tmp);
        system(CLEAR);
        break;
      case 4:
        system(CLEAR);
        long tmp2;
        cout<<"Menú de búsqueda empleando 'tree binary search method'"<<endl<<endl;
        cout<<"Por favor, digite su ID de Médico: ";
        cin>>tmp2;
        searchMedico(arbolMedico,tmp2);
        system(CLEAR);
        break;
      case 5:
        system(CLEAR);
        buscarHistorialVisitas();
        system(CLEAR);
        break;
      case 6:
        system(CLEAR);
        mostrarPacientes();
        system(CLEAR);
        break;
      case 7:
        system(CLEAR);
        menuRegistrar();
        system(CLEAR);
        break;
      case 8:
        break;
      default:
        cout<<"¡Opción no valida!"<<endl;
        break;
    }
  }while(opt!=8);
  cout<<"Saliendo del menú"<<endl;
}

void menuRegistrar(){
  int opt=0;
  do{
      cout<<".:Bienvendio al sistema de registros de historias médicas:."<<endl<<endl;
      cout<<"[_OPCIONES_]"<<endl;
      cout<<"1. Añadir registros de historial a un paciente"<<endl;
      cout<<"2. Salir"<<endl;
      cout<<">>";
      cin>>opt;
      cin.get();

      switch(opt){
        case 1:
          system(CLEAR);
          registrarConsulta();
          system(CLEAR);
          break;
        case 2:
          break;
        default:
          cout<<"¡Opción no válida!"<<endl;
      }
  }while(opt!=2);
  cout<<"Saliendo del menú de registro de historias clínicas..."<<endl;
}

void addPaciente(){
  //nodoPacientes* nuevo=(nodoPacientes*)malloc(sizeof(nodoPacientes));
  nodoPacientes* nuevoAddPaciente=new nodoPacientes();

  srand (time(NULL));
  int num1=(rand() % 100000 +1);

  //Aquí nos cersioramos que el ID creado no se repita
  comprobarExistenciaIDPaciente(num1);
  while(n!=0){
    srand (time(NULL));
    int num1=(rand() % 100000 +1);
    n=0;
    comprobarExistenciaIDPaciente(num1);
  }

  //Inserción del ID del paciente al arbol
  insertarNodoArbolPaciente(arbolPaciente,num1);

  //captura de datos
  cout<<"Paciente ID #"<<num1<<endl<<endl<<endl;
  nuevoAddPaciente->ID_paciente=num1;
  cout<<"Por favor, digite los nombres del paciente: ";
  cin.getline(nuevoAddPaciente->nombres_Paciente,sizeof(nuevoAddPaciente->nombres_Paciente));
  cout<<endl;
  cout<<"Por favor, digite los apellidos del paciente: ";
  cin.getline(nuevoAddPaciente->apellidos_Paciente,sizeof(nuevoAddPaciente->apellidos_Paciente));
  cout<<endl;
  cout<<"Por favor, digite el correo electronico del paciente: ";
  cin.getline(nuevoAddPaciente->correo_electronicoPaciente,sizeof(nuevoAddPaciente->correo_electronicoPaciente));
  cout<<"\n";
  cout<<"Por favor, digite el sexo del paciente ('H':Hombres - 'M':Mujeres) : ";
  cin>>nuevoAddPaciente->sexo_Paciente;
  cin.get();
  cout<<"\n";
  cout<<"Por favor, digite el numero del paciente: ";
  cin>>nuevoAddPaciente->numero_celularPaciente;
  cin.get();
  cout<<endl;
  cout<<"Por favor, digite el estado civil del paciente (Soltero/Casado/Divorciado/En una relacion/Indefinido): ";
  cin.getline(nuevoAddPaciente->estado_civilPaciente,sizeof(nuevoAddPaciente->estado_civilPaciente));

  if(primeroPacientes==NULL){
    primeroPacientes=nuevoAddPaciente;
    primeroPacientes->sig=NULL;
    ultimoPacientes=nuevoAddPaciente;
  }
  else{
    ultimoPacientes->sig=nuevoAddPaciente;
    nuevoAddPaciente->sig=NULL;
    ultimoPacientes=nuevoAddPaciente;
  }
  cout<<"Paciente ingresado con éxito"<<endl;
  system(PAUSE);
}

void addMedico(){
  //nodoMedicos* nuevo=(nodoMedicos*)malloc(sizeof(nodoMedicos));
  nodoMedicos* nuevoAddMedico=new nodoMedicos();

  srand (time(NULL));
  int num2=(rand() % 100000 +1);

  //Aquí nos cersioramos que el ID creado no se repita
  comprobarExistenciaIDMedico(num2);
  while(m!=0){
    srand (time(NULL));
    num2=(rand() % 100000 +1);
    m=0;
    comprobarExistenciaIDMedico(num2);
  }

  //Inserción del ID del paciente al arbol
  insertarNodoArbolMedico(arbolMedico,num2);

  //captura de datos
  cout<<"Medico ID #"<<num2<<endl<<endl<<endl;
  nuevoAddMedico->ID_Medico=num2;
  cout<<"Por favor, digite los nombres del medico: ";
  cin.getline(nuevoAddMedico->nombres_Medico,sizeof(nuevoAddMedico->nombres_Medico));
  cout<<endl;
  cout<<"Por favor, digite los apellidos del medico: ";
  cin.getline(nuevoAddMedico->apellidos_Medico,sizeof(nuevoAddMedico->apellidos_Medico));
  cout<<endl;
  cout<<"Por favor, digite el correo electronico del medico: ";
  cin.getline(nuevoAddMedico->correo_electronicoMedico,sizeof(nuevoAddMedico->correo_electronicoMedico));
  cout<<"\n";
  cout<<"Por favor, digite el sexo del medico ('H':Hombres - 'M':Mujeres) : ";
  cin>>nuevoAddMedico->sexo_Medico;
  cin.get();
  cout<<"\n";
  cout<<"Por favor, digite el numero del medico: ";
  cin>>nuevoAddMedico->numero_celularMedico;
  cin.get();
  cout<<endl;
  cout<<"Por favor, digite el estado civil del medico (Soltero/Casado/Divorciado/En una relacion/Indefinido): ";
  cin.getline(nuevoAddMedico->estado_civilMedico,sizeof(nuevoAddMedico->estado_civilMedico));

  if(primeroMedicos==NULL){
    primeroMedicos=nuevoAddMedico;
    primeroMedicos->sig=NULL;
    ultimoMedicos=nuevoAddMedico;
  }
  else{
    ultimoMedicos->sig=nuevoAddMedico;
    nuevoAddMedico->sig=NULL;
    ultimoMedicos=nuevoAddMedico;
  }
  cout<<"Médico ingresado con éxito"<<endl;
  system(PAUSE);
}

void searchPaciente(nodoArbolPaciente *search_arbol_paciente, long n){

  if(search_arbol_paciente==NULL){
    cout<<"No se encuentra el paciente porque el arbol está vacío o no se ha registrado dicho paciente"<<endl;
    system(PAUSE);
  }
  else if(search_arbol_paciente->dato==n){

    //
    nodoPacientes* actual=new nodoPacientes();
    actual=primeroPacientes;
    if(primeroPacientes!=NULL){
      while(actual!=NULL){
        if(actual->ID_paciente==search_arbol_paciente->dato){
          cout<<"¡Paciente encontrado!"<<endl<<endl;
          cout<<".:Impresión de datos del paciente:."<<endl;
          cout<<"Nombres: "<<actual->nombres_Paciente<<endl;
          cout<<"Apellidos: "<<actual->apellidos_Paciente<<endl;
          cout<<"Correo Electrónico: "<<actual->correo_electronicoPaciente<<endl;
          cout<<"Sexo: "<<actual->sexo_Paciente<<endl;
          cout<<"Número: "<<actual->numero_celularPaciente<<endl;
          cout<<"Estado civil: "<<actual->estado_civilPaciente<<endl;
          system(PAUSE);
        }
        actual=actual->sig;
      }
    }
    else{
      cout<<"Error"<<endl;
      system(PAUSE);
    }
  }
  else if(n<search_arbol_paciente->dato){
    searchPaciente(search_arbol_paciente->izq,n);
  }
  else{
    searchPaciente(search_arbol_paciente->der,n);
  }
}

void searchMedico(nodoArbolMedico *search_arbol_medico, long n){
  if(search_arbol_medico==NULL){
    cout<<"No se encuentra el medico porque el arbol está vacío o no se ha registrado dicho Médico"<<endl;
    system(PAUSE);
  }
  else if(search_arbol_medico->dato==n){
    //
    nodoMedicos* actual=new nodoMedicos();
    actual=primeroMedicos;
    if(primeroMedicos!=NULL){
      while(actual!=NULL){
        if(actual->ID_Medico==search_arbol_medico->dato){
          cout<<"¡Medico encontrado!"<<endl<<endl;
          cout<<".:Impresión de datos del paciente:."<<endl;
          cout<<"Nombres: "<<actual->nombres_Medico<<endl;
          cout<<"Apellidos: "<<actual->apellidos_Medico<<endl;
          cout<<"Correo Electrónico: "<<actual->correo_electronicoMedico<<endl;
          cout<<"Sexo: "<<actual->sexo_Medico<<endl;
          cout<<"Número: "<<actual->numero_celularMedico<<endl;
          cout<<"Estado civil: "<<actual->estado_civilMedico<<endl;
          system(PAUSE);
        }
        actual=actual->sig;
      }
    }
    else{
      cout<<"Error"<<endl;
      system(PAUSE);
    }
  }
  else if(n<search_arbol_medico->dato){
    searchMedico(search_arbol_medico->izq,n);
  }
  else{
    searchMedico(search_arbol_medico->der,n);
  }
}

void comprobarExistenciaIDPaciente(long num){
  //nodoPacientes* actual=(nodoPacientes*)malloc(sizeof(nodoPacientes));
  nodoPacientes* actual=new nodoPacientes();
  actual=primeroPacientes;
  if(primeroPacientes!=NULL){
    while(actual!=NULL){
      if(actual->ID_paciente==num){
        n=1;
      }
      actual=actual->sig;
    }
  }
  else{
    cout<<"\n";
  }
  cout<<"ID_Paciente Aceptado"<<endl;
}

void comprobarExistenciaIDMedico(long num){
  //nodoMedicos* actual=(nodoMedicos*)malloc(sizeof(nodoMedicos));
  nodoMedicos* actual=new nodoMedicos();
  actual=primeroMedicos;
  if(primeroMedicos!=NULL){
    while(actual!=NULL){
      if(actual->ID_Medico==num){
        m=1;
      }
      actual=actual->sig;
    }
  }
  else{
    cout<<"\n";
  }
  cout<<"ID_Medico Aceptado"<<endl;
}


//Funciones del arbol
nodoArbolPaciente *crearNodoArbolPaciente(long n){
  nodoArbolPaciente *nuevo_nodo=new nodoArbolPaciente();

  nuevo_nodo->dato=n;
  nuevo_nodo->der=NULL;
  nuevo_nodo->izq=NULL;

  return nuevo_nodo;
}

nodoArbolMedico *crearNodoArbolMedico(long n){
  nodoArbolMedico *nuevo_nodo=new nodoArbolMedico();

  nuevo_nodo->dato=n;
  nuevo_nodo->der=NULL;
  nuevo_nodo->izq=NULL;

  return nuevo_nodo;
}

//Funciones de insertar en el arbol de médicos y pacientes los ID's
void insertarNodoArbolPaciente(nodoArbolPaciente *&arbol_paciente, long n){
  if(arbol_paciente==NULL){//Si el nodo del arbol de pacientes está vacío
    nodoArbolPaciente *nuevo_nodo=crearNodoArbolPaciente(n);
    arbol_paciente=nuevo_nodo;
  }
  else{ //Si el arbol de pacientes tiene un nodo o más
    long valorRaizPaciente=arbol_paciente->dato;//obtenemos el valor de la raiz
    if(n<valorRaizPaciente){//Si el elemento es menor a la raiz, insertamos en izq
      insertarNodoArbolPaciente(arbol_paciente->izq,n);
    }
    else{//Si el elemento es mayor a la raiz, insertamos en der
      insertarNodoArbolPaciente(arbol_paciente->der,n);
    }
  }
}
void insertarNodoArbolMedico(nodoArbolMedico *&arbol_medico, long n){
  if(arbol_medico==NULL){//Si el nodo del arbol de medicos está vacío
    nodoArbolMedico *nuevo_nodo=crearNodoArbolMedico(n);
    arbol_medico=nuevo_nodo;
  }
  else{ //Si el arbol de medicos tiene un nodo o más
    long valorRaizMedico=arbol_medico->dato;//obtenemos el valor de la raiz
    if(n<valorRaizMedico){//Si el elemento es menor a la raiz, insertamos en izq
      insertarNodoArbolMedico(arbol_medico->izq,n);
    }
    else{//Si el elemento es mayor a la raiz, insertamos en der
      insertarNodoArbolMedico(arbol_medico->der,n);
    }
  }
}

void mostrarPacientes(){
  //
  nodoPacientes* actual=new nodoPacientes();
  actual=primeroPacientes;
  cout<<".:Lista de pacientes:."<<endl;
  if(primeroPacientes!=NULL){
    while(actual!=NULL){
      system(CLEAR);
      cout<<"ID del paciente: "<<actual->ID_paciente<<endl<<endl;
      cout<<"Nombres: "<<actual->nombres_Paciente<<endl;
      cout<<"Apellidos: "<<actual->apellidos_Paciente<<endl;
      actual=actual->sig;
      system(PAUSE);
    }
  }
  else{
    cout<<"\nLista de pacientes vacía";
  }
}

void registrarConsulta(){
  nodoMedicos *nuevo_nodo_Medico=new nodoMedicos();
  nodoPacientes *nuevo_nodo_Paciente=new nodoPacientes();
  //variables locales a la función
  long codigoMedico;
  long codigoPaciente;

  nuevo_nodo_Medico=primeroMedicos;
  nuevo_nodo_Paciente=primeroPacientes;


  cout<<"Por favor, digite su código de médico: ";
  cin>>codigoMedico;
  if(primeroMedicos!=NULL){
    while(nuevo_nodo_Medico!=NULL){
      if(nuevo_nodo_Medico->ID_Medico==codigoMedico){
        cout<<"Por favor, digite el código del paciente al cual va a registrar historia: ";
        cin>>codigoPaciente;
        if(primeroPacientes!=NULL){
          while(nuevo_nodo_Paciente!=NULL){
            if(nuevo_nodo_Paciente->ID_paciente==codigoPaciente){
              //Aquí se hace todo
              cout<<"\nMédico tratante: "<<nuevo_nodo_Medico->nombres_Medico<<endl;
              ++nuevo_nodo_Paciente->cont_Visitas;
              cout<<"Número de visita: "<<nuevo_nodo_Paciente->cont_Visitas<<endl;
              cout<<"Doctor, por favor digite el turno del paciente: ";
              cin>>nuevo_nodo_Paciente->turno;
              cin.get();
              cout<<"\nDoctor, por favor digite la fecha de atención en formato dd/mm/yyyy: ";
              cin.getline(nuevo_nodo_Paciente->fecha_Atencion,sizeof(nuevo_nodo_Paciente->fecha_Atencion));
              cout<<endl;
              cout<<"\nDoctor, por favor digite el diagnostico: ";
              cin.getline(nuevo_nodo_Paciente->diagnostico_Paciente,sizeof(nuevo_nodo_Paciente->diagnostico_Paciente));
              cout<<endl;
              cout<<"\nDoctor, por favor digite el tratamiento: ";
              cin.getline(nuevo_nodo_Paciente->tratamiento_Paciente,sizeof(nuevo_nodo_Paciente->tratamiento_Paciente));
              cout<<endl;
              cout<<"\nDoctor, por favor digite la fecha de la proxima cita de control en formato dd/mm/yyyy: ";
              cin.getline(nuevo_nodo_Paciente->fecha_Proxcontrol,sizeof(nuevo_nodo_Paciente->fecha_Proxcontrol));
              cout<<endl;
              cout<<"Registro finalizado"<<endl;
            }
            else if(nuevo_nodo_Paciente->sig==NULL){
              cout<<"Paciente no encontrado"<<endl;
            }
            else{
              cout<<endl;
            }
            nuevo_nodo_Paciente=nuevo_nodo_Paciente->sig;
          }
        }
        else{
          cout<<"No hay pacientes registrados aún"<<endl;
        }
      }
      else if(nuevo_nodo_Medico->sig==NULL){
        cout<<"Médico no encontrado"<<endl;
      }
      else{
        cout<<endl;
      }
      nuevo_nodo_Medico=nuevo_nodo_Medico->sig;
    }
  }
  else{
      cout<<"No hay médicos registrados aún"<<endl;
  }

}

void buscarHistorialVisitas(){
  if(primeroPacientes==NULL){
    cout<<"Aún no hay pacientes registrados"<<endl;
  }
  else{
    nodoPacientes *nuevo=new nodoPacientes;
    nuevo=primeroPacientes;
    while(nuevo!=NULL){
      if(nuevo->cont_Visitas>0){
        cout<<"Paciente ID: "<<nuevo->ID_paciente<<endl<<endl;
        cout<<"Nombre del paciente: "<<nuevo->nombres_Paciente<<endl;
        cout<<"Número de visitas: "<<nuevo->cont_Visitas<<endl;
        cout<<"Turno número: "<<nuevo->turno<<endl;
        cout<<"Fecha de atención"<<nuevo->fecha_Atencion<<endl;
        cout<<"Diagnóstico por el médico tratante: "<<nuevo->diagnostico_Paciente<<endl;
        cout<<"Tratamiento a realizar: "<<nuevo->tratamiento_Paciente<<endl;
        cout<<"Fecha del próximo control: "<<nuevo->fecha_Proxcontrol<<endl;
        system(PAUSE);
      }
      else{
        cout<<"Paciente ID: "<<nuevo->ID_paciente<<"No ha registrado visitas"<<endl;
      }
    nuevo=nuevo->sig;
    }
  }
  cout<<"Historial terminado"<<endl;
  system(PAUSE);
}
