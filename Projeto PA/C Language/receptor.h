void receptor(char CommPortName[]){

 /*-------------------------------------------Abrindo a porta Serial-------------------------------------------*/
    HANDLE hComm;                         // Carrega a Porta Serial
    BOOL Status;                          // Status de várias operações
    DWORD dwEventMask;                    // Mascara de evento para engatilhar
    char TempChar;                        // Caracter temporario
    char SerialBuffer[256];               // Um buffer contendo as informações
    DWORD NoBytesLidos;                   // Bytes lidos pelo ReadFile()
    int i = 0;

    hComm = CreateFileA(CommPortName,                 // Nome da porta
                        GENERIC_READ | GENERIC_WRITE, // Permissões de Ler/Escrever (receber ou enviar bytes)
                        0,                            // Sem compartilhamento das portas
                        NULL,                         // Sem segurança
                        OPEN_EXISTING,                // Abrir apenas portas existentes
                        0,                            // Inputs e Outputs não sobrepostos
                        NULL);                        // Null para dispositivos Comm

    if (hComm == INVALID_HANDLE_VALUE){
        printf("Erro ao tentar abrir a porta %s", CommPortName);
    }else{
        printf("Porta %s aberta com sucesso", CommPortName);
    }

 /*------------------------------------Configurando os parametros da porta Serial------------------------------*/

    DCB dcbParametrosSerial = { 0 };                       // Inicializa a estrutura DCB | Utilizada para definir as configurações da porta
    dcbParametrosSerial.DCBlength = sizeof(dcbParametrosSerial);

    Status = GetCommState(hComm, &dcbParametrosSerial);    // Retona as configs atuais da porta | Pede um ponteiro para a estrutura DCB
                                                           // Se a função funcionar corretamente, retornará um valor diferente de 0
                                                           // Senão, retornará 0

    if (Status == FALSE){
        new_print("\n Erro ao tentar requisitar estrutura DCB",10);
    }

    dcbParametrosSerial.BaudRate = CBR_9600;              // Define BaudRate = 9600
    dcbParametrosSerial.ByteSize = 8;                     // Define ByteSize = 8
    dcbParametrosSerial.StopBits = ONESTOPBIT;            // Define StopBits = 1 | Indica quandos bits serão usados para indicar o fim dos dados
    dcbParametrosSerial.Parity   = NOPARITY;              // Define Parity = None | Pesquisa o que é paridade de bits, muito longo de explicar aqui

    Status = SetCommState(hComm, &dcbParametrosSerial);   // Aplica na porta as configurações definidas no DCB (Acima)

    if (Status == FALSE){
        new_print("\n Erro ao tentar aplicar a estrutura DCB",10);
    }else{
        new_print("\n   | Estrutura DCB aplicada com sucesso",15);
        printf("\n      | Baudrate = %d", dcbParametrosSerial.BaudRate);
        Sleep(15);
		printf("\n      | ByteSize = %d", dcbParametrosSerial.ByteSize);
        Sleep(15);
		printf("\n      | StopBits = %d", dcbParametrosSerial.StopBits);
        Sleep(15);
		printf("\n      | Parity   = %d", dcbParametrosSerial.Parity);
    }

 /*------------------------------------Configurando os Timeouts (Limites de tempo)------------------------------*/

    COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout         = 50;  // Define o intervalo máximo de leitura entre um byte e outro.

	timeouts.ReadTotalTimeoutConstant    = 50;  // Usado para calcular o Timeout total das operações de leitura.
                                                // Cada operação de leitura soma esse valor ao ReadTotalTimeoutMultiplier

	timeouts.ReadTotalTimeoutMultiplier  = 10;  // Usado para calcular o Timeout total das operações de leitura.
                                                // Multiplica a si mesmo pelo número de bytes a serem lidos.

	timeouts.WriteTotalTimeoutConstant   = 50;  // Parecido com o ReadTotalTimeoutConstant mas para escrever.
	timeouts.WriteTotalTimeoutMultiplier = 10;  // Parecido com o ReadTotalTimeoutMultiplier mas para escrever.

	if (SetCommTimeouts(hComm, &timeouts) == FALSE){
		new_print("\n   Erro ao tentar definir os Timeouts",10);
    }else{
		new_print("\n\n   | Timeouts da porta Serial definidos com sucesso.",15);
    }

 /*------------------------------------Configurando a mascara de recessão------------------------------*/

    Status = SetCommMask(hComm, EV_RXCHAR); // Configura o Windows para monitorar o dispositivo serial para recessão de caracteres

    if (Status == FALSE){
        new_print("\n\n Erro ao difinir CommMask",10);
    }else{
        new_print("\n\n   | CommMask dfinido com sucesso",15);
    }

 /*------------------------------------Configurando o evento WaitComm()------------------------------*/

    printf("\n\n Esperando por recepcao de dados");

    Status = WaitCommEvent(hComm, &dwEventMask, NULL); // Espera pelos caracteres a serem recebidos

    /*------------------------------------Aqui o programa vai esperar até receber uma informação------------------------------*/

    if(Status == FALSE){
        new_print("\n Erro ao configurar WaitCommEvent()",10);
    }else{ // Se o WaitCommEvent() == TRUE leia os valores a serem recebidos usando ReadFile()
        new_print("\n\n Dados recebidos",15);
        do{
            Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesLidos, NULL);
            SerialBuffer[i] = TempChar;
            i++;
        } while (NoBytesLidos > 0);
    }

    /*------------------------------------Printando os dados lidos no console------------------------------*/

    printf("\n\n    ");
    int j = 0;
    for (j = 0; j < i-1; j++){
        printf("%c", SerialBuffer[j]);
    }

    printf("\n\nPressione \"Enter\" para continuar");
    getchar();
    system("cls");

    
    CloseHandle(hComm);//Fecha a porta Serial

}