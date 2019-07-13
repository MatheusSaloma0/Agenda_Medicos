								/**************************************************************************						
								Trabalho Computacional 2 - Programação II - 2017/02 - Ciência da Computação
											Grupo : Leonardo Khoury Picoli e Matheus Salomão
								**************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define dim 50		//Tamanho máximo de um nome
#define h 10		//Números de horário em um dia
#define d 5			//Número de dias em uma semana
#define med 10		//Número máximo de médicos
#define pac 100		//Número máximo de pacientes
#define semanas 4
#define dadinho "dadosMedicos.txt"
#define passinho "listaPacientes-Semana"

	typedef struct{					
		char nome[dim];
		int id;
		char especialidade[dim];
		int agenda[h][d];
	}agMedico;

	typedef struct{
		char nome[dim];
		int id;
		long int fone;
		int idade;
		char medico[dim];
	}cliente; 

void retiraEspacos (char string[]){		//Função que retira os espaços de uma string
	int i,posicao,len = strlen(string);
 	posicao = 0;
    
	for (i = 0; i < len; i++) {			//O contador i percorre cada elem. da string 
		if (string[posicao] == ' ') {	//Sempre que um espaço é identificado o valor de posição é atualizado
			posicao++;
		}
		string[i] = string[posicao];	//Um espaço sendo identificado ou não o caracter de string[posicao] sera passado para string[i]
    posicao++;							//Caso um espaço seja identificado todos os carateres a frente serão movidos p/ esquerda
	}
}

void iniciarAgenda(int (*agenda)[d]){		//Função para iniciar a agenda dos medicos
	int i,j;								//Ela definirá todos os elem. como zero
	for(i=0;i<h;i++){
		for(j=0;j<d;j++){
			agenda[i][j]=0;
		}
	}
	for(j=0;j<5;j++){						//Definição do horário de almoço como -1
		agenda[4][j]=-1;
	}
}

void zerarAgenda(int (*agenda)[d]){			//Essa função elimina todas as consultas marcadas de uma semana 
	int i,j;								//Essa função é usada no fim de cada semana 
	for(i=0;i<h;i++){						
		for(j=0;j<d;j++){
			if(agenda[i][j]!=-1 && agenda[i][j]!=0){
				agenda[i][j]=0;
			}
		}
	}
}

void criarArquivos(agMedico *medico,int k){ 				//Cria um arquivo com o nome do medico
	FILE *arq;
	char nomeSemEspacos[dim];
	int i;
		
		strcpy(nomeSemEspacos,medico[k].nome);				//Seu nome é retirado da struct correspondente e é passado p/ uma string
		retiraEspacos(nomeSemEspacos);						//Os espaços de seu nome são retirados
		strcat(nomeSemEspacos,".txt");						
	
		arq = fopen(nomeSemEspacos,"w");					//Seus dados iniciais são impressos no arquivo

		fprintf(arq,"Medico: %s \n",medico[k].nome);
		fprintf(arq,"Id: %d \n",medico[k].id);
		fprintf(arq,"Especialidade: %s \n\n",medico[k].especialidade);
		fprintf(arq,"Quadro de consultas semanais\n");

		fclose(arq);
}

void escreveAgenda(agMedico *medico,int k,int semana){			//Função responsável por imprimir a agenda do medico no arquivo 
	FILE *arq;
	char nomeSemEspacos[dim];
	int i,j;	
												//Assim como na função anterior,
	strcpy(nomeSemEspacos,medico[k].nome);		//Seu nome é retirado da struct correspondente e é passado p/ uma string
	retiraEspacos(nomeSemEspacos);				//Os espaços de seu nome são retirados
	strcat(nomeSemEspacos,".txt");
	arq = fopen(nomeSemEspacos,"a");			//As informações são escritas no final do arquivo

	fprintf(arq,"\nSemana %d:\n\n",semana);

	fprintf(arq,"        2a  3a  4a  5a  6a\n");
	fprintf(arq," ");
	for (i=0;i<h;i++){
		fprintf(arq,"%3d-%d ",i+8,i+9);
		for (j=0;j<d;j++){
			fprintf(arq,"%3d ",medico[k].agenda[i][j]);
		}
		fprintf(arq,"\n");
	}
	fprintf(arq,"\n");
	fclose(arq);
}	

void recebeMedico(agMedico *medico,int k,FILE *arq,int *ultimo,char *corrigir){		//Lê no arquivo as informações dos medicos
	int i,j;
	char c,correcao[dim];
	
	fscanf(arq,"%[^\n]",medico[k].nome);		 
	if(*corrigir>64 && *corrigir<91){			// Durante o trabalho tivemos problemas com a presença ou não de espaços no	
		correcao[0]=c;											// fim do arquivo dados médicos, essa parte do codigo permite que ele funcione
		correcao[1]='\0';                   // define o final da string
		strcat(correcao,medico[k].nome);		// com ou sem espaços no fim do arquivo, e tendo ou não restrição de horário
		strcpy(medico[k].nome,correcao); 
		*corrigir=0;
	}
	fscanf(arq,"%d \n",&medico[k].id);
	fscanf(arq,"%[^\n] ",medico[k].especialidade);	
	while((c=fgetc(arq))!='\n' && c!=EOF && (c<65 || c>90) ){
		j=(c-48)-2;		//Converte char pra int e subtrai 2 visto que '2a' corresponde ao dia 0
		c=fgetc(arq);
		while((c=fgetc(arq))!='\n'){
			if (c != ' '){
				i=10*(c-48);	//Pega o algarismo da dezena
				c=fgetc(arq);
				i+=(c-48);		//Pega o algarismo da unidade
				medico[k].agenda[i-8][j] = -1;
			}
		}
	}
	if((c>=65 && c<=90)){
		*corrigir=c;
	}
	if(c==EOF){
		*ultimo=1;
	}
}	

void especialidadeDaClinica (agMedico *medico,char (*especialidades)[dim],int nMed){
	int i,j,k=0,jatem=0;

	for(i=0;i<nMed;i++){							//O contador i procura a especialidade de cada um dos medicos
		jatem=0;
		for(j=0;j<i;j++){							//Esse for procura no vetor de strings especialidades[med][dim] (presente na main)
			if(strcmp(medico[i].especialidade,especialidades[j]) == 0){	//Se a especialidade do medico em questão ja foi adicionada
				jatem=1;												//Caso tenha sido, a variável jatem é alterada
			}
		}
		if(jatem == 0){													//Fazendo com que a operação dentro do if de add a especialidade
			strcpy(especialidades[k],medico[i].especialidade);			// ao vetor especialidades não seja executada
			k++;
		}
	}
}

int disponivel(agMedico *medico,int k){			//Verifica se a agenda do medico está disponível 
	int i,j;
	
	for(i=0;i<10;i++){
		for(j=0;j<5;j++){
			if(medico[k].agenda[i][j]==0){		//Se houver algum elem. igual a zero a função retorna 1(TRUE)
				return 1;
			}
		}
	}				
	return 0;									//Caso contrário, 0(FALSE)
}

int idadePaciente (int dia, int mes, int ano) {
	int dAtual,mAtual,aAtual;

	struct tm *local;		//O struct tm ja pertence a biblioteca time.h, ele armazena segundos, minutos, hora, dia, mes, ano,...	
	time_t t;				//O tipo de dado time_t é definido em C para armazenar valores de tempo
	t = time(NULL);			//Para retornar o horário do momento de execução do programa
	local = localtime(&t);	//A função localtime é necessária para preencher o struct tm com os dados do momento atual de execução do prog.

	dAtual = local-> tm_mday;			//Corresponde ao dia
	mAtual = local-> tm_mon+1;			//Corresponde ao mes (foi somado 1 porque os meses vão de 0-11)
	aAtual = local-> tm_year+1900; 		//Corresponde ao ano (foi necessário somar 1900 pois o ano 0 corresponde ao ano 1900)
	
	if( (mes<mAtual) || (mes==mAtual && dia<=dAtual) ){
		return (aAtual - ano);						// Já fez aniversário esse ano
	}else {
		return (aAtual-ano-1);						// Ainda não fez aniversário esse ano
	}
}

int faixaEtaria(int idade){		//Indica a faixa etária de uma pessoa de acordo com sua idade
	if(idade<=25){				
		return 0;				//Faixa 1
	}
	if(idade<=50){
		return 1;				//Faixa 2
	}
	if(idade<=75){
		return 2;				//Faixa 3
	}
	if(idade>=76){
		return 3;				//Faixa 4
	}
}

void recebeCliente (cliente *paciente,int k,FILE *arq,int *ultimo){		//Lê no arquivo as informações dos pacientes da clínica
	int dia, mes, ano,acabou=0;
	char c;
	
	fscanf(arq,"%[^\n]",paciente[k].nome);		//Lê no arquivo o nome do paciente
	fscanf(arq,"%d \n",&paciente[k].id);		//Lê no arquivo a ID do paciente
	fscanf(arq,"%ld",&paciente[k].fone);		//Lê no arquivo o telefone do paciente
	fscanf(arq,"%d %d %d \n",&dia,&mes,&ano);	//Lê no arquivo o dia, mês e ano do aniversário do paciente
	paciente[k].idade = idadePaciente (dia,mes,ano);	//A partir da data de nascimento do paciente, é calculada sua idade
	fscanf(arq,"%[^\n]",paciente[k].medico);			//Lê no arquivo o médico que o paciente deseja se consultar
	c=fgetc(arq);					//Ler quebra de linha
	if(c==EOF){													
		acabou=1;					//Artifício utilizado para NUNCA dar erro
	}
	if((c=fgetc(arq))==EOF && acabou==1){
		*ultimo=1;
	}
}		

void auxiliar_marcarconsulta(agMedico *medico,cliente *paciente,char (*especialidades)[dim],int *naoAtendidos,int nMed,int j,int k,int semana,int (*contSemanas)[med]){
int l,c,m;
	if( disponivel(medico,j) ){						//Verifica se o medico esta disponivel    
		do{
			l=rand()%10;							//Num. aleatório para a linha da agenda	
			c=rand()%5;								//Num. aleatório para a coluna da agenda
		}while(medico[j].agenda[l][c]!=0);
		medico[j].agenda[l][c]=paciente[k].id;		//O ID do paciente é add ao horário escolhido

		for(m=0;m<nMed;m++){	
			if( strcmp(medico[j].especialidade,especialidades[m]) == 0){
				contSemanas[semana-1][m]++;			//Dados que serão utilizados para saber os atendidos por semana
			}																														
		}

	}else{
		for(m=0;m<nMed;m++){	
			if(strcmp(medico[j].especialidade,especialidades[m]) == 0){		//Cada vez que um paciente não é atendido
				naoAtendidos[m]++;											//soma-se 1 no vetor não-atendidos
			}																//Esse vetor será usado (e informado)
		}																	//No arquivo dadosClinica.txt
		printf("Não foi possível marcar a consulta do paciente %s\n",paciente[k].nome);
	}

}

void marcarconsulta(agMedico *medico,cliente *paciente,FILE *arq,int semana,int *popular,char (*especialidades)[dim], int (*contEspecialidades)[med],int (*contSemanas)[med],int *naoAtendidos,int nMed){
	int nPac,j=0,k,ultimo=0,m;
	char ListaPacientes[27];
	srand(time(NULL));										//Necessário para que as consultas sejam marcadas de modo aleatório
	
	sprintf(ListaPacientes,"%s%d.txt",passinho,semana);		//A função sprintf junta variáveis e monta tudo em uma string
	arq = fopen(ListaPacientes,"r");
	if(arq==NULL){											//Caso o arquivo não seja encontrado
		printf("Não foi possível encontrar o arquivo %s \n",ListaPacientes);
		exit(1);
	}
	for(k=0;ultimo==0;k++){									//Recebimento dos dados de cada paciente 
		recebeCliente(paciente,k,arq,&ultimo);
	}
	fclose(arq);
	nPac=k-1;				//Num. de pacientes

	for(k=0;k<nPac;k++){
		for(j=0;j<nMed;j++){
			if(strcmp(paciente[k].medico,medico[j].nome)==0){	//Compara se o nome do medico é igual ao medico desejado pelo paciente
				auxiliar_marcarconsulta(medico,paciente,especialidades,naoAtendidos,nMed,j,k,semana,contSemanas);
				popular[j]++;			//Dados que serão utilizados posteriormente para saber o medico mais popular
				for(m=0;m<nMed;m++){	
					if( strcmp(medico[j].especialidade,especialidades[m]) == 0){
						contEspecialidades[faixaEtaria(paciente[k].idade)][m]++;	//Dados que serão utilizados para saber a 
					}																//especialidade mais popular
				}
			}
		}
	}
	for(int w=0;w<nMed;w++){					//Procedimento para o início de uma nova semana
		escreveAgenda(medico,w,semana);
		zerarAgenda(medico[w].agenda);
	}
}

int maiorDalinha(int (*matriz)[med],int linha,int nMed){ 	//Responde o maior elemento da linha x de uma matriz de inteiros 
	int i,maior,mudou;
	
	maior = matriz[linha][0];
	mudou = 0;												//Mudou receberá a posicao do maior elem.
	for(i=1;i<nMed;i++){
		if (matriz[linha][i]>maior){
			mudou = i;
			maior = matriz[linha][i];
		}
	}
	if(matriz[linha][mudou]==0){				//Casos em que não existe um maior elem.
		return -1;								//Como essa função foi criada para ser usada na agenda dos medicos 
	}else{										// A possibilidade do maior num. como zero não foi considerada
		return mudou;
	}
}

void popularPorFaixa (int (*contEspecialidades)[med],char (*copiaEspecialidades)[dim],FILE *arq,int nMed) {
	int	i,j,pos;								//Calcula a especialidade mais popular(maior elem.) de uma faixa etária(linha)
	
	for (i=0;i<4;i++){
		pos = maiorDalinha(contEspecialidades,i,nMed);
		if(pos<0){
			fprintf(arq,"Faixa %d -> Não possui pacientes nessa faixa etária\n",i+1);	
		}else{
			fprintf(arq,"Faixa %d -> %s \n",i+1,copiaEspecialidades[pos]);
		}
	}

}		

void quicksort (int vetor[dim], int inicio, int fim, char (*especialidades)[dim],int *naoAtendidos){
	int pivo, aux, i, j, meio;
	char auxiliar[dim];

								//Um exemplar da função Quicksort que ordena os elem. em ordem decresente
	i = inicio;
	j = fim;
	meio = (int) ((i + j) / 2);
	pivo = vetor[meio];
   
	do{
		while (vetor[i] > pivo) 
			i = i + 1;
		while (vetor[j] < pivo) 
			j = j - 1;
      
		if(i <= j){
			aux = vetor[i];
			vetor[i] = vetor[j];
			vetor[j] = aux;

			aux = naoAtendidos[i];
			naoAtendidos[i] = naoAtendidos[j];
			naoAtendidos[j] = aux;
		
			strcpy(auxiliar,especialidades[i]);
			strcpy(especialidades[i],especialidades[j]);
			strcpy(especialidades[j],auxiliar);
      i = i + 1;
      j = j - 1;
		}
	}while(j > i);
   
	if(inicio < j) quicksort(vetor, inicio, j,especialidades,naoAtendidos);
 	if(i < fim) quicksort(vetor, i, fim,especialidades,naoAtendidos);   
}

int converte(char *espec, char (*especialidades)[dim], int nMed){		// Dada uma especialidade e um vetor de strings
	int k,i;															// Se as strings comparadas forem iguais, a função
																		// Devolve a posição dessa string no vetor especialidades
	for(k=0;k<nMed;k++){
		for(i=0;i<nMed;i++){											
			if(strcmp(espec,especialidades[k]) == 0){					
				return k;
			}
		}
	}
}

void dadosDaClinica (int *popular,agMedico *medico,int (*contEspecialidades)[med],int (*contSemanas)[med], int naoAtendidos[med], char (*especialidades)[dim],int nMed){
	
	int i,k,maior=0,ranking[med];
	char copiaEspecialidades[med][dim];
	
	for(i=0;i<nMed;i++){
		ranking[i]=0;
	}

	FILE *arq;
	arq = fopen("dadosClinica.txt","w");

	for(k=0;k<med;k++){				//Definação do maior num. de consultas de um medico
		if(popular[k]>maior){
			maior=popular[k];
		}
	}

	fprintf(arq,"Dados da Clínica: \n \n");

	fprintf(arq,"Medicos mais popular(es): \n\n");
	for(k=0;k<nMed;k++){
		if(popular[k]==maior){		//Verificação dos medicos cujo num. de consultas é igual ao valor de maior
			fprintf(arq,"%s \n",medico[k].nome);
		}
	}
	
	fprintf(arq,"\nPopularidade de cada Especialidade: \n\n");
	
	for(i=0;i<nMed;i++){
		for(k=0;k<4;k++){
			ranking[i]+=contEspecialidades[k][i];//Soma a linha da matriz contEspecialidades
		}
	}
	
	for(i=0;i<nMed;i++){			//Copia o vetor especialidades
		strcpy(copiaEspecialidades[i],especialidades[i]);
	}
	
	quicksort(ranking,0,nMed-1,especialidades,naoAtendidos);//Ordenação p/ que os resultados sejam apresentados em ordem decrescente

	for(i=0;i<nMed;i++){
		if(strlen(especialidades[i])>0){
			fprintf(arq,"%s \n Procura: %d \n Total de atendidos: %d \n Atendimentos por semana: \n",especialidades[i],ranking[i],ranking[i]-naoAtendidos[i]);
			for(k=0;k<semanas;k++){
				fprintf(arq," Semana %d: %d \n",k+1,contSemanas[k][converte(especialidades[i],copiaEspecialidades,nMed)]);
			}
			fprintf(arq,"\n\n");
		}
	}

	fprintf(arq,"Especialidades mais populares por Faixa Etária: \n\n");	

	popularPorFaixa(contEspecialidades,copiaEspecialidades,arq,nMed);

	fclose(arq);	

}

void inicializar(int popular[med],int (*contEspecialidades)[med],int (*contSemanas)[med],int naoAtendidos[med],int nMed){
	int i,j;
								//Redefine popular,nao-atendidos, contEspecialidades e contSemanas
	for(i=0;i<nMed;i++){
		popular[i]=0;						
		naoAtendidos[i]=0;
	}		
	
	for(i=0;i<4;i++){
		for(j=0;j<nMed;j++){
			contEspecialidades[i][j]=0;
		}
	}
	
	for(i=0;i<semanas;i++){
		for(j=0;j<nMed;j++){
			contSemanas[i][j]=0;
		}
	}
	
}
		
int main(){
	int k,ultimo=0,i,nMed,popular[med],contEspecialidades[4][med],contSemanas[semanas][med],naoAtendidos[med]; 
	agMedico medico[med];													//especialidades[med][dim] e contEspecialidades[4][med]
	cliente paciente[pac];													//Foram criadas para serem usadas de maneira relacionada
	FILE *arq;																//Isto é, seus componentes são correspondetes
	char especialidades[med][dim],corrigir=0;								//Isso se torna mais evidente a partir da análise das funções
	

	char dadosMedicos[]=dadinho;
	arq = fopen(dadosMedicos, "r");
	if(arq==NULL){
		printf("Não foi possível encontrar o arquivo %s \n",dadosMedicos);
		exit(1);
	}
	for(k=0;ultimo==0;k++){
		iniciarAgenda(medico[k].agenda);
		recebeMedico(medico,k,arq,&ultimo,&corrigir);
	}
	fclose(arq);
	
	if(strlen(medico[k-1].nome)==0){
		k--;
	}	
	nMed=k;			//Número de médicos

	for(k=0;k<nMed;k++){
		criarArquivos(medico,k);
	}
	
	especialidadeDaClinica(medico,especialidades,nMed);
	
	inicializar(popular,contEspecialidades,contSemanas,naoAtendidos,nMed);

	for(k=1;k<semanas+1;k++){
		marcarconsulta(medico,paciente,arq,k,popular,especialidades,contEspecialidades,contSemanas,naoAtendidos,nMed);	
	}

	dadosDaClinica(popular,medico,contEspecialidades,contSemanas,naoAtendidos,especialidades,nMed);

return 0;
}
