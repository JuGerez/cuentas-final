/// Funciones de Cuenta:
stCuentas cargaUnaCuenta(char nombreArchivo[]) ///Carga 1 sola cuenta, retorna stCuentas cuenta.
{

    int opcion;
    stCuentas Cuenta;
    stCliente Cliente;
    char dni[10];

    printf("Conoce el Id de Cliente o lo quiere buscar por nro de Dni? \n");
    printf("1. Lo conozco. \n");
    printf("2. Lo tengo que buscar. \n");
    scanf("%d", &opcion);

    system("cls");
    system("pause");

    while(opcion > 2 || opcion < 1) //Bucle para que no pueda pasar si no elige una opcion valida.
    {
        printf("Seleccione una opcion valida: \n");
        printf("1. Conozco el Id del Cliente. \n");
        printf("2. Lo tengo que buscar. \n");
        fflush(stdin);
        scanf("%d", &opcion);
    }

    system("cls");
    getch();

    if ( opcion == 2)
    {
        do
        {
            printf("Ingrese el Dni del Cliente que quiere buscar: \n");
            fflush(stdin);
            gets(&Cliente.dni);
            Cuenta.idCliente = mostrarClientesDNIDevolverId(nombreArchivo,Cliente.dni);
            ///Para mostrar y seleccionar clientes con DNI, retorna id.
            ///Si hay duplicados te muestra todos los clientes que hay con ese DNI y podes ingresar el id correpondiente.
        }
        while(Cuenta.idCliente == -2);/// No existe el dni.

        if(Cuenta.idCliente==-1) /// Opcion de escape porque no sabe el dni.
        {
            Cuenta.id=-2;
            return Cuenta;
        }
    }

    system("cls");
    getch();

    while(opcion==1) /// Eligio la opcion 1 porque sabe el Id.
    {
        printf("\nIngrese el Id de Cliente: \n");
        fflush(stdin);
        scanf("%d", &Cuenta.idCliente);
        traducirIdClientePorDNI(nombreArchivo, Cuenta.idCliente, dni);  // Traduce el id ingresado a un dni en stClientes para chequear que exista.
        if(dni[0] == '\0')// Si el dni no existe te deja el string vacio.
        {
            printf("\nNo existe ese Id de Cliente, ingrese 1 para seguir probando u otro numero para salir");
            fflush(stdin);
            scanf("%d", &opcion);
            if(opcion!=1)
            {
                Cuenta.id=-2;
                return Cuenta;//Se retorna una Cuenta con id -2, para filtrarla mas tarde y que no quede cargada en el archivo.
                //A esta cuenta no se le siguen agregando datos.
            }
        }
        else //Si existe te confirma el ingreso.
        {
            printf("Se ingreso el IdCliente %d que se corresponde con el DNI %s", Cuenta.idCliente, dni);
            opcion = 0; // Lo igualamos a 0 para dejarlo limpio y poder usarlo mas adelante.
        }
    }

    system("cls");
    getch();

    printf("\nSeleccione el tipo de cuenta que quiere abrir: \n");
    printf("1. Caja de ahorro en pesos. \n");
    printf("2. Caja de ahorro en dolares. \n");
    printf("3. Cuenta corriente en pesos. \n");
    scanf("%d", &Cuenta.tipoDeCuenta);

    system("cls");
    getch();

    while(Cuenta.tipoDeCuenta > 3 || Cuenta.tipoDeCuenta < 1)// Bucle para que ingrese una opcion valida.
    {
        printf("Opcion no valida. \n");
        printf("Seleccione un tipo de cuenta valido: \n");
        printf("1. Caja de ahorro en pesos. \n");
        printf("2. Caja de ahorro en dolares. \n");
        printf("3. Cuenta corriente en pesos. \n");
        scanf("%d", &Cuenta.tipoDeCuenta);
    }

    //Se cargan costos iniciales que después mes a mes pueden ir variando.
    if(Cuenta.tipoDeCuenta == 1)
    {
        Cuenta.costoMensual = 0;
    }

    if(Cuenta.tipoDeCuenta == 2)
    {
        Cuenta.costoMensual = 45;
    }

    if(Cuenta.tipoDeCuenta == 3)
    {
        Cuenta.costoMensual = 16200;
    }

    Cuenta.saldo = 0; //Lo iniciamos en 0 por ser cuenta nueva.

    Cuenta.eliminado = 1;//Al estar creandola el estado es activo.

    return Cuenta;
}

void cargaCuentaArch(char nombreArchivo[], char nombreArchivoCliente[], stCuentas cuenta)///Pasa al archivo el stCuenta cuenta de CargaUnaCuenta.
{
    if(cuenta.id!=-2) /// Filtro para que si la carga fue cortada al principio no se cargue en el archivo.
    {
        int cantidadDeCuenta=0;
        char dni[10];
        FILE* archivo = fopen(nombreArchivo, "ab");

        if(archivo)
        {
            cuenta.id = cuentaRegistrosGral(nombreArchivo,sizeof(stCuentas)) + 1;//Te retorna la cantidad de regitros ya guardados
            //le sumamos 1 para convertirlo en id.

            /// Generador del nroCuenta = cuenta.nroCuenta.
            cantidadDeCuenta = contarCuentasPorIdClientePorTipo(nombreArchivo,cuenta.idCliente,cuenta.tipoDeCuenta);
            //Retorna la cantidad de cuentas de cada tipo que tiene un cliente.
            traducirIdClientePorDNI(nombreArchivoCliente,cuenta.idCliente,dni);
            //Traducimos el idCliente que tenemos a dni para usarlo en el nro de cuenta.
            cuenta.nroCuenta=componerNroCuenta(cantidadDeCuenta,dni,cuenta.tipoDeCuenta,1);
            //Concatenamos el nro d sucursal (1, un digito), cantidad de cuentas x tipo (3 digitos), 4 primeros nros del dni y tipo de cuenta (1, 2 o 3).
            muestraUnaCuenta(cuenta);

            system("cls");
            getch();

            fwrite(&cuenta, sizeof(stCuentas), 1, archivo);

            fclose(archivo);
        }
    }
}

int componerNroCuenta(int cantidadDeCuenta,char dni [],int tipoDeCuenta, int sucursal)/// Componemos el nro de cuenta.
{
    //Concatenamos el nro d sucursal (1, un digito), cantidad de cuentas x tipo (3 digitos), 4 primeros nros del dni
    //y tipo de cuenta (1, 2 o 3).

    int dni0 = dni[0] - '0';
    int dni1 = dni[1] - '0';
    int dni2 = dni[2] - '0';
    int dni3 = dni[3] - '0';
    printf("\nGenerando Nro de Cuenta");
    printf("\nSucursal: %d",sucursal);
    printf("\nCantidad de cuentas por tipo: %d",cantidadDeCuenta);
    printf("\nPrimer Nro dni: %d",dni0);
    printf("\nSegundo Nro dni: %d",dni1);
    printf("\nTercer Nro dni: %d",dni2);
    printf("\nCuarto Nro dni: %d",dni3);
    printf("\nTipo de cuenta: %d",tipoDeCuenta);

    int numeroCuentaInt= tipoDeCuenta+dni3*10+dni2*100+dni1*1000+dni0*10000+cantidadDeCuenta*100000+sucursal*100000000;
    printf("\nNumero de Cuenta: %d",numeroCuentaInt);
    return numeroCuentaInt;
}

int contarCuentasPorIdClientePorTipo(char nombreArchivo[],int idCliente, int tipoDeCuenta) //Retorna la cantidad de cuentas de por tipo que tiene el cliente.
{
    int contador=0;
    stCuentas cuenta;
    FILE* archivo = fopen(nombreArchivo,"rb");
    if(archivo)
    {
        while(fread(&cuenta,sizeof(stCuentas),1,archivo)>0)
        {
            if(cuenta.idCliente==idCliente)
            {
                if(cuenta.tipoDeCuenta==tipoDeCuenta)
                {
                    contador=contador+1;
                }
            }
        }
        fclose(archivo);
    }
    return contador;
}

void encabezadoCuenta()
{
    printf("-----------------------------------------------------------------------------------------------------------------\n");
    printf("%-10s | %-10s | %-20s | %-20s | %-10s | %-10s\n",
           "DNI", "NroCuenta", "TipoDeCuenta", "CostoMensual", "Saldo", "Estado");
    printf("-----------------------------------------------------------------------------------------------------------------\n");
}

void muestraUnaCuentaListadoConId(stCuentas Cuenta, char archivoCliente[])//Te muestra una cuenta en base al id.
{
    char dni[10];
    traducirIdClientePorDNI(archivoCliente,Cuenta.idCliente,dni);//Para mostrarle el dni en el listado y no el id que no es facil de relacionar al cliente.

    printf("%-10s | %-10d | %-20d | %-20.2f | %-10.2f | %-10s| %-20d \n",
           dni,
           Cuenta.nroCuenta,
           Cuenta.tipoDeCuenta,
           Cuenta.costoMensual,
           Cuenta.saldo,
           Cuenta.eliminado == 1 ? "Inactiva" : "Activa",Cuenta.id);
}

void muestraUnaCuenta(stCuentas Cuenta) ///Muestra cuenta antes de guardar en archivo.
{

    printf("\nLos datos de la cuenta cargada son:");
    printf("\nEl Id: %d", Cuenta.id);
    printf("\nEl Id del Cliente: %d", Cuenta.idCliente);
    printf("\nEl Nro de Cuenta: %d", Cuenta.nroCuenta);
    printf("\nEl tipo de Cuenta: %d", Cuenta.tipoDeCuenta);
    printf("\nEl costo mensual de la Cuenta es: %d", Cuenta.costoMensual);
    printf("\nEl saldo actual de la Cuenta es: %d", Cuenta.saldo);
    if(Cuenta.eliminado == 1)
    {
        printf("\nLa cuenta no esta activa.");
    }
    else
    {
        printf("\nLa cuenta esta activa.");
    }
}

int verificarNroDeCuenta(int numeroCuenta, char nombreCuenta[]) ///Chequea la existencia del nro cuenta, para el entrelazado con los movimientos.
{
    int bandera = 0;
    int idCuenta = -1;
    stCuentas cuenta;
    FILE* archivo = fopen(nombreCuenta,"rb");
    if (archivo)
    {
        while (fread(&cuenta,sizeof(stCuentas),1,archivo)>0)
        {

            if(cuenta.nroCuenta==numeroCuenta)
            {
                bandera = bandera -1;
                idCuenta=cuenta.id;
            }
        }

        fclose(archivo);
    }
    if(bandera <-1)
    {
        idCuenta=bandera;
    }
    return idCuenta;// -1 si no existe, idCuenta si existe y de -1 para abajo si hay más de una.
}

int ingresarNroCuentaValido(char nombreArchivo[]) ///En movimintos ingresan el nro de cuenta al que quieren cargarle el movimiento.
{
    int idCuenta;
    int nroCuenta;
    int opcion=1;
    printf("Ingrese Nro de Cuenta\n");
    scanf("%d",&nroCuenta);
    idCuenta=verificarNroDeCuenta(nroCuenta, nombreArchivo); /// -1 si no existe, idCuenta si existe y de -1 para abajo si hay más de una.

    system("cls");
    getch();

    while(opcion==1 && idCuenta==-1)
    {
        printf("Ingrese Nro de Cuenta valido\n");
        scanf("%d",&nroCuenta);
        idCuenta=verificarNroDeCuenta(nroCuenta, nombreArchivo);//Vuelve a comprobarlo.

        system("cls");
        getch();

        if(idCuenta == -1)
        {
            printf("\nNro de Cuenta no valido, desea probar con otro Nro de cuenta?");
            printf("\n1. Si, quiero probar con otro Nro de Cuenta. \n2. No, quiero volver al menu anterior.\n Opcion: ");
            scanf("%d",&opcion);

        }/// Le damos la opcion de salir del bucle, el idcuenta permanece en -1.
    }

    if(idCuenta < -1)/// El numero de cuenta es valido pero hay mas de una con ese nro.
    {
        idCuenta = resolverCuentasDuplicadas(nombreArchivo,nroCuenta); // Devuelve el idCuenta final a usar o -1 si escapo.
    }
    if(opcion == 2)
    {
        idCuenta = -1;
    }

    return idCuenta; /// Retorna -1 si escapo o el idCuenta.
}

int resolverCuentasDuplicadas(char nombreArchivo[], int nroCuenta) ///Para seleccionar el id correcto al cargarle el movimiento en caso de que haya un nro de cta repetido.
{

    int id;
    int* ids = (int*)malloc(sizeof(int));
    int validos=0;
    int opcion;
    stCuentas cuenta;
    FILE* archivo = fopen(nombreArchivo,"rb");
    if (archivo)
    {

        while(fread(&cuenta,sizeof(stCuentas),1,archivo)>0)
        {
            if(cuenta.nroCuenta == nroCuenta)
            {
                ids[validos] = cuenta.id;
                validos++;
                printf("\n%d - Cuenta ---------------------------------------------------------------------- ", validos);
                muestraUnaCuenta(cuenta);
                printf("\n --------------------------------------------------------------------------------");
                ids = (int*)realloc(ids,(validos+1)*sizeof(int));
            }
        }
        fclose(archivo);
    }

    do
    {
        printf("\nSeleccione el Id de Cuenta o elija 0 para salir: ");
        scanf("%d",&opcion);
    }
    while (opcion > validos || opcion < 0);

    if (opcion == 0)
    {
        id = -1; // Escapa.
    }
    else
    {
        id = ids[opcion-1];// Le asignamos a id la opcion elegida, como usamos validos se le resta 1.
    }
    free(ids);
    return id; // Retorna el id cuenta o -1 si escapa.
}

void cambiarEstadoCuenta(char nombreArchivo[], int idCuenta, int estado)///Cambia el estado de la cta.
{

    stCuentas Cuenta;
    FILE* archivo = fopen(nombreArchivo, "r+b");
    if(archivo)
    {
        fseek(archivo, idCuenta*sizeof(stCuentas), 0);
        fread(&Cuenta, sizeof(stCuentas), 1, archivo);
        Cuenta.eliminado = estado;
        fseek(archivo, -1*sizeof(stCuentas), 1);
        fwrite(&Cuenta, sizeof(stCuentas), 1, archivo);

        fclose(archivo);
    }
}

void modificarCostoCuentaPorId(char nombreArchivo[], int idCuenta, float nuevoCosto)///En base al idCuenta cambiamos el costo.
{
    stCuentas Cuenta;
    FILE* archivo = fopen(nombreArchivo, "r+b");
    if(archivo)
    {
        fseek(archivo, idCuenta*sizeof(stCuentas), 0);
        fread(&Cuenta, sizeof(stCuentas), 1, archivo);
        modificarCostoCuenta(&Cuenta, nuevoCosto);//Guardamos el nuevo costo.
        fseek(archivo, -1*sizeof(stCuentas), 1);
        fwrite(&Cuenta, sizeof(stCuentas), 1, archivo);

        fclose(archivo);
    }
}

void modificarCostoCuenta(stCuentas* cuenta, float nuevoCosto)///Modifica el costo mensual.
{
    cuenta->costoMensual=nuevoCosto;
}

void muestraPorIdCuenta(char nombreArchivo[], int idCuenta)///Muestra una cta por idCuenta en texto(usarla para mostrar uno solo).
{
    stCuentas cuenta;
    FILE *archivo = fopen(nombreArchivo, "rb");

    if (archivo)
    {
        fseek(archivo,idCuenta*sizeof(stCuentas),0);
        fread(&cuenta,sizeof(stCuentas), 1, archivo);
        muestraUnaCuenta(cuenta);
        fclose(archivo);
    }
}

void mostrarCuentasxIdCliente(char archivoCuenta[], int idCliente, char archivoCliente[])///Muestra una cuenta en base al id.
{
    stCuentas cuenta;
    FILE * archivo = fopen(archivoCuenta,"rb");
    if(archivo)
    {
        encabezadoCuenta();
        while(fread(&cuenta,sizeof(stCuentas),1,archivo)>0)
        {
            if(cuenta.idCliente==idCliente)
            {
                muestraUnaCuentaListadoConId(cuenta,archivoCliente);
            }
        }
        fclose(archivo);
    }
}

int mostrarCuentasxIdClienteSeleccionar(char archivoCuenta[], int idCliente) ///Poder ver todos los idCuenta en base a un idCliente.
{
    stCuentas cuenta;
    int id = -1;
    int* ids = (int*)malloc(sizeof(int));
    int validos = 0;
    int opcion = 4;
    FILE * archivo = fopen(archivoCuenta,"rb");
    if(archivo)
    {
        while(fread(&cuenta,sizeof(stCuentas),1,archivo)>0)
        {
            if(cuenta.idCliente == idCliente)
            {
                ids[validos] = cuenta.id;
                validos++;
                printf("\n%d - Cuenta: ", validos);
                muestraUnaCuenta(cuenta);
                ids = (int*)realloc(ids,(validos + 1)*sizeof(int));
            }
        }
        fclose(archivo);
    }
    /// Si validos = 0 no hay cuentas con ese idCliente, validos = 1 hay una sola, validos > 1 esa cant de cuentas para ese idCliente.

    if(validos > 1)
    {
        printf("\nSeleccione el Nro de Cuenta o elija 0 para salir: ");
        scanf("%d", &opcion);
        while (opcion>validos || opcion < 0)
        {
            printf("\nSeleccione una opcion valida entre 0 y %d: ", validos);
            scanf("%d", &opcion);
        }
    }
    if (opcion != 0) /// mantiene -1 porque opcion fue 0
    {
        id=ids[opcion-1]; /// Eligio la cta correspondiente, se le resta uno pq se lo habiamos sumado para el printf.
    }
    if (validos == 0)
    {
        id=-2; /// El idCliente no tiene cuentas.
    }
    if (validos == 1)
    {
        id=ids[0];
    }
    free(ids);
    return id; ///Retorna el idCuenta o -2 si no tiene cuentas ese idCliente.
}

void buscarCuentaModificarSaldo(char nombreArchivoCu[], int idCuenta, float importe)///Te posicionas en el idCuenta al q le queres modificar el saldo y lo modificas.
{
    stCuentas cuenta;
    FILE * archivo = fopen(nombreArchivoCu,"r+b");
    if( archivo)
    {
        fseek(archivo,(idCuenta)*sizeof(stCuentas),0);
        fread(&cuenta,sizeof(stCuentas),1,archivo);

        cuenta.saldo = cuenta.saldo+importe;

        fseek(archivo,(idCuenta)*sizeof(stCuentas),0);
        fwrite(&cuenta,sizeof(stCuentas),1,archivo);
        fclose(archivo);
    }
}

void mostrarArchivoCuentas(char archivoCuenta[], char archivoCliente[], int cantidad)///Mientras haya cuentas guardadas en el archivo, las muestra en listado.
{
    stCuentas cuenta;
    FILE * archivo = fopen(archivoCuenta,"rb");
    if(archivo)
    {
        encabezadoCuenta();
        while(fread(&cuenta,sizeof(stCuentas),1,archivo)>0)
        {
            muestraUnaCuentaListadoConId(cuenta,archivoCliente);
        }
        fclose(archivo);
    }
}
