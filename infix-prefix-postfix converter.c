#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAX 30
/*
* Structure type
*/
struct Operands{
  char data[MAX];
  struct Operands *next;
};
typedef struct Operands operands;
typedef operands *opr_ptr;

struct Operators{
  char data;
  struct Operators *next;
};
typedef struct Operators operators;
typedef operators *ops_ptr;

struct Evaluation{
  int value;
  struct Evaluation *next;
};
typedef struct Evaluation evaluation;
typedef evaluation *eval_ptr;


// Conversion function prototype
char *prefix_infix(char input[],int length);
char *postfix_infix(char input[],int length);
char *infix_postfix(char input[],int length);
char *infix_prefix(char input[],int length);

char *strrevv(char *str);
int isoperator(char input); // determine an operator
int extype(char input[]);
int menu(char input[],FILE **outfile); 
int pows (int x,int n); //power operation
int precedence(char input); // precedence of the operators
int evaluate(char input[],int length); //evaluation

//PUSH OPERATION
void push_opr(char input[],opr_ptr *first_opr); //push operands stack
void push_ops(char input[],ops_ptr *first_ops); //push operators stack
void push_eval(char input[],eval_ptr *first_eval); //push evaluation stack

//POP OPERATION
void pop_ops(ops_ptr *first_ops); //pop operands stack
void pop_opr(opr_ptr *first_opr); //pop operators stack
void pop_eval(eval_ptr *first_eval); ////pop evaluation stack
/*
* Error handling Fucntion Prototype
*/  
int errorcheck(char input[],int type); //check if any error occur
void displayerror(int location,char input[],int type);// display error



int main() {
  FILE *out;

  char input[MAX],sel;
  while(1){
      fflush(stdin);//clear input buffer
      puts("Note: Input digit from 0 - 99 and not more than 6 operands and 5 operators in the expression");  
      memset(input,0,sizeof(input));
      printf("Enter your expression here:");
      fgets(input,MAX,stdin); // prompt input
      //getchar();
      out=fopen("out.txt","a"); 
      if(!menu(input,&out)){
      fclose(out); // End program if menu return 0
      return 0; 
      }
  }
  return 0;
}
int menu(char input[],FILE **outfile){
  /*
  * EXPRESSION TYPE: Determine the type of the expression
  */
  int ex_type;
  char sel;
  ex_type=extype(input);
  /*
  * ERROR CHECKING: Function errorcheck will perform
  * the required step to check the errors and return
  * an error so the switch case will display the error
  * to the user 
  */
  int error=0; 
  error=errorcheck(input,ex_type);
  switch(error){
    case 1:
    puts("Error: Invalid Spacing\n");
    break;
    case 2:
    puts("Error: Not Enough Operands\n");
    break;
    case 3:
    puts("Error: Not Enough Operators\n");
    break;
    case 4:
    puts("Error: Expression is too long\n");
    break;
    case 5:
    puts("Error: Exceed input limit\n");
    break;
    case 6:
    puts("Error: Imbalanced Bracket\n");
    break;
    case 7:
    puts("Error: Invalid Expression\n");
    break;
  }
  /*
  * Prompt user to input the expression again or 
  * to end the program if there's error
  */
  if (error>0){
    while(1){
    puts("Do you wish to Insert your expression again ?\n(A) Yes\n(B) No");
    scanf(" %c",&sel); //Get user input
    getchar(); // Remove newline character
    switch(sel){
      case 'A':
      return 1;
      break;
      case 'B':
      return 0;
      break; //break 
      default:
      puts("Wrong Expression");
      puts("Do you wish to Insert your expression again ?\n(A) Yes\n(B) No");
      scanf(" %c",&sel); //Get user input
      getchar(); // Remove newline character
    }
    }
    
    return 0;
  }
  /*
  * USER MENU: Let user select an action
  */
  char menu_sel;
  printf("\nPlease select an action: \n"
                    "A. Convert to infix, prefix or postfix \n"
                    "B. Evaluate expression\n"
                    "C. Exit Program\n?: ");
  scanf(" %c",&menu_sel);
  getchar(); //Remove newline character
  if(menu_sel!='A' && menu_sel!='B' && menu_sel !='C'){
    printf("Invalid Selection\n");
    printf("\nPlease select an action: \n"
                    "A. Convert to infix, prefix or postfix \n"
                    "B. Evaluate expression\n"
                    "C. Exit Program\n?: ");
  scanf(" %c",&menu_sel);
  getchar(); //Remove newline character
  }
  /*
  * To show user the type of expression and if
  * the user wish to convert the expression and
  * save it into a file
  *
  * Variable Description
  * ex_type  : Determine the type of expression
  * con_type : User desired conversion type
  */
  char con_type;
  if(menu_sel=='A'){
    switch (ex_type){
    case 0:
    printf("This is an Infix Expression\nDo you wish to convert into\n(A) Prefix\n(B) Postfix\n(C) Both\n");
    break; // break case 0
    case 1:
    printf("This is an Prefix Expression\nDo you wish to convert into\n(A) Infix\n(B) Postfix\n(C) Both\n");
    break; // break case 1
    case 2:
    printf("This is an Postfix Expression\nDo you wish to convert into\n(A) Infix\n(B) Prefix\n(C) Both\n");
    break; // break case 2
    }
    printf("?:");
    scanf(" %c",&con_type);
    getchar(); //Remove newline character
    while(con_type!='A' && con_type!='B' && con_type !='C'){
      printf("ERROR: Invalid Selection, please insert again\n");
      printf("?:");
      scanf(" %c",&con_type);
      getchar(); //Remove newline character
    }
    /*
    * The beginning of the conversion part
    *
    * Variable Description
    * temp        : Temporary output
    * outptr      : Output pointer
    * ex_type     : Determine the type of expression
    * con_type    : User desired conversion type
    * output_file : Temporary input for file printing  
    */    
    char temp[MAX]={'\0'},*outptr=NULL;
    char output_file[MAX];
    strcpy(output_file,input);

    switch(ex_type){
      case 0:
          switch(con_type){
            case 'A':
                strcpy(temp,infix_prefix(input,strlen(input))); //infix to prefix function
                printf("Prefix: %s\n",temp); // print prefix
                fprintf(*outfile,"Infix To Prefix of %s:%s\n",output_file,temp); //write file
            break;
            case 'B': 
                strcpy(temp,infix_postfix(input,strlen(input))); // infix to postfix function
                printf("Postfix: %s\n",temp); //print postfix
                fprintf(*outfile,"Infix To Postfix of %s:%s\n",output_file,temp); //write file
            break;
            case 'C':
                strcpy(temp,infix_prefix(input,strlen(input))); // infix to prefix function
                printf("Prefix: %s\n",temp); //print prefix
                fprintf(*outfile,"Infix To Prefix %s:%s\n",output_file,temp);  //write file
                outptr=infix_postfix(temp,strlen(temp)); // infix to postfix function
                printf("Postfix: %s\n",outptr);// print postfix
                fprintf(*outfile,"Infix To Postfix %s:%s\n",output_file,outptr);//write file
            break;
          }
      break; //break ex_type case 0 statement
      case 1: // ex_type case 1
          switch(con_type){
            case 'A':
                strcpy(temp,prefix_infix(input,strlen(input))); //prefix to infix Function
                printf("Infix: %s\n",temp); //print infix
                fprintf(*outfile,"Prefix To Infix of %s:%s\n",output_file,temp);// write file
            break;
            case 'B': 
                strcpy(temp,prefix_infix(input,strlen(input)));// prefix to infix function
                outptr=infix_postfix(temp,strlen(temp));// infix to postfix function
                fprintf(*outfile,"Prefix To Postfix of %s:%s\n",output_file,outptr);// write file
            break;
            case 'C':
                strcpy(temp,prefix_infix(input,strlen(input))); // prefix to infix function
                printf("Infix: %s\n",temp);// print infix
                fprintf(*outfile,"Prefix To Infix of %s:%s\n",output_file,temp); //write file
                outptr=infix_postfix(temp,strlen(temp));// infix to postfix function
                printf("Postfix: %s\n",outptr);// print postfix 
                fprintf(*outfile,"Prefix To Postfix of %s:%s\n",output_file,outptr);// write file
            break;
          }
      break; // break ex_type case 1 statement
      case 2: //ex_type case 2
          switch(con_type){
            case 'A':
                strcpy(temp,postfix_infix(input,strlen(input))); // postfix to infix
                printf("Infix: %s\n",temp);// print infix
                fprintf(*outfile,"Postfix To Infix of %s:%s\n",output_file,temp);// write file
            break;
            case 'B':
                strcpy(temp,postfix_infix(input,strlen(input)));// postfix to infix function
                outptr=infix_prefix(temp,strlen(temp));// infix to prefix function
                printf("Prefix: %s\n",outptr);//print prefix
                fprintf(*outfile,"Postfix To Prefix of %s:%s\n",output_file,outptr);//write file
            break;
            case 'C':
                strcpy(temp,postfix_infix(input,strlen(input)));//postfix to infix function
                printf("Infix: %s\n",temp);//print infix
                fprintf(*outfile,"Postfix To Infix of %s:%s\n",output_file,temp);//write file
                outptr=infix_prefix(temp,strlen(temp));//infix to prefix function
                printf("Prefix: %s\n",outptr);//print prefix
                fprintf(*outfile,"Postfix To Prefix of %s:%s\n",output_file,outptr);//write file
            break;
          }
      break; //break ex_type case 2 statement
    }
  }
  /*
  * IF USER SELECTED EVALUATION
  */
  if(menu_sel=='B'){
  char output_eval[MAX],temp[MAX]={'\0'},*outptr=NULL, output_file[MAX];
  strcpy(output_file,input);

  switch (ex_type){
  case 0:
  outptr=infix_postfix(input,strlen(input));//infix to postfix function
  sprintf(output_eval,"Result: %d",evaluate(outptr,strlen(outptr))); //write result
  printf("%s\n",output_eval);//print result
  fprintf(*outfile,"Evaluation of %s:%s",output_file,output_eval);//write file
  break;

  case 1:
  outptr=prefix_infix(input,strlen(input));//prefix to infix function
  strcpy(temp,outptr);// string copy string to temp
  outptr=infix_postfix(temp,strlen(temp));//infix to postfix function
  sprintf(output_eval,"Result: %d",evaluate(outptr,strlen(outptr)));//write result
  printf("%s\n",output_eval);//print result
  fprintf(*outfile,"Evaluation of %s:%s",output_file,output_eval);//write file
  break;

  case 2:
  sprintf(output_eval,"Result: %d",evaluate(input,strlen(input)));//write result
  printf("%s\n",output_eval);//print result
  fprintf(*outfile,"Evaluation of %s:%s",output_file,output_eval);//write file
  break;
  }
  }
  if(menu_sel=='C'){
  return 0;
  }
  char input_sel;
  printf("Do you wish to input again?\n(A) Yes (B) No\n");
  printf("?:");
  scanf(" %c",&input_sel);
  getchar(); //Remove newline character
  while(input_sel!='A' && input_sel!='B'){
    printf("Inavlid Selection\n");
    printf("Do you wish to input again?\n(A) Yes (B) No\n");
    printf("?:");
    scanf(" %c",&input_sel);
    getchar();//Remove newline character
  }
  if(input_sel=='A'){
  return 1;
  }
  else if(input_sel =='B'){
    return 0;
  }
  return -1;
}
/*
* EVALUATION OF THE EXPRESSION
*
* Variable Description:
*
* temp       = Temporary expression for linked list
* op1,op2    = Temporary operand to sprintf into a string
* first_eval = Head of linked list 
* 
* Function Description:
* push_eval = Evaluation struct pushing
* pop_eval  = Pop an element from linked list (Evaluation)
*/
int evaluate(char input[],int length){
  eval_ptr first_eval=NULL;
  int op1=0,op2=0,i;
  char temp[5];
  for(i=0;i<length;i++){
    if(isdigit(input[i])){ // if number is 1 digit
      if(isdigit(input[i+1])){ //if number is 2 digit
        sprintf(temp,"%c%c ",input[i],input[i+1]);//insert both number into temp array
        push_eval(temp,&first_eval); //push number into eval function
        i++; //+1 since it is already checked
      }
      else{
        sprintf(temp,"%c ",input[i]); //insert number into temp array
        push_eval(temp,&first_eval); //push number into eval function
      }
    }
    else if(isoperator(input[i])){
      op1=first_eval->value; //op1 as temporary
      pop_eval(&first_eval); //pop value op1 in linked list
      op2=first_eval->value; //op2 as temporary
      pop_eval(&first_eval); //pop value op2 in linked list
      switch(input[i]){
        case '+':
            sprintf(temp,"%d ",(op1+op2)); //addition
            push_eval(temp,&first_eval);  //push value into linked list
            break;
        case '-':
            sprintf(temp,"%d ",(op2-op1)); //subtraction
            push_eval(temp,&first_eval);  //push value into linked list
            break;
        case '*':
            sprintf(temp, "%d ",(op2*op1));// multiplication
            push_eval(temp,&first_eval);  //push value into linked list
            break;
        case '/':
            sprintf(temp, "%d ",(op2/op1));// division
            push_eval(temp,&first_eval);// push value into linked list
            break;
        case '^':
            sprintf(temp,"%d ",pows(op2,op1));// power operation
            push_eval(temp,&first_eval);// push value into linked list
            break;
      }
    }
  }
  return first_eval->value; // return final value
}
int errorcheck(char input[],int type){
  int i,j,count,loc=0;
  char input2[MAX],check[MAX];
  // copy operators and operands without '(' , ')' and NULL
  for (i=0,j=0;i<=strlen(input);i++){
    if(input[i-1]=='(' || input[i-1]==')'){
      continue;
    }
    if (input[i]!='\0' && input[i]!='(' && input[i]!=')' )
      input2[j++]=input[i];
  }

  // check brackets
  int openbrac=0,closebrac=0,locbrac=0;;
  for(i=0;i<=strlen(input);i++){
    if (input[i]=='('){
      if(openbrac==0){
        locbrac=i; // determine the location of the first bracket
      }
      openbrac++; // number of open brackets
    }
    if(input[i]==')'){
    closebrac++; //number of closing brackets
    }
    if(closebrac>openbrac){
      displayerror(locbrac,input,1);
      return 6;
    }
  }
  if (openbrac!=closebrac){
    displayerror(locbrac,input,1);
    return 6;
  }
  // value check ,'r' for operands, 'o' for operators and 's' for space 
  for (i=0,j=0,count=0;i<=strlen(input);i++){
    if(isoperator(input2[i])){
      check[j++]='o';//put o if it is an operator
    }
    if(isdigit(input2[i])){
      if(isdigit(input2[i+1])){ // make sure the digit is within 0-99
        if(isdigit(input2[i+2])){ 
          displayerror(i+2,input,2); //Display error if it's larger than 0-99
          return 5; //return error
        }
        i++;// i++ since it is already checked
      }
      check[j++]='r'; //put r if it is operands
    }
    if(isspace(input2[i])){
      check[j++]='s'; //put s if it is a space
    }
  }
  count=j;
  // check spacing
  for (i=1;i<=count;i+=2){ //will return error if user did not space
    if(check[i]!='s'){
    displayerror(i-1,input,2);
    return 1;
    }
  }
  // check operands and operators
  int rcheck=0,ocheck=0;
  for (i=0;i<=count;i++){
    if(check[i]=='r') 
    rcheck++;//operands +1
    if(check[i]=='o')
    ocheck++;//operators +1
  }
  if (ocheck!=rcheck-1){ 
    if(ocheck>rcheck-1){ //not enough operands
      return 2;
    }
    if(rcheck-1>ocheck){//not enough operators
      return 3;
    }
  }
  if (rcheck>6){ // will return error if the expression is too long
    return 4;
  }
  return 0;
}
/*
* CONVERT FROM PREFIX TO INFIX
* reference = https://www.web4college.com/converters/postfix-prefix-to-infix.php;
*
* Variable Description:
*
* temp      = Temporary expression for linked list
* o1,o2     = Temporary operand to sprintf into a string
* opr_count = Numbers of operands
* out       = Output
* first_opr = Head of linked list
* 
* Function Description:
* strrevv  = String reverse funciton
* push_opr = Operands struct pushing
* pop_opr  = Pop an element from linked list (Operands)
*/
char *prefix_infix(char input[],int length){
  opr_ptr first_opr=NULL;
  static char out[MAX]={'\0'};
  memset(out,'\0',MAX);
  char o1[MAX]={'\0'},o2[MAX]={'\0'},temp[MAX]={'\0'};
  int i,opr_count=0;
  strrevv(input);
  for(i=0;i<=length;i++){
    if(isdigit(input[i])){ //if number is 1 digit
      if(isdigit(input[i+1])){ //if number is 2 digit
        sprintf(temp,"%c%c ",input[i+1],input[i]); //copy num into temp
        i++;//i++ since it is checked
      }
      else{
        sprintf(temp,"%c ",input[i]);//copy num into temp
      }
    push_opr(temp,&first_opr);//push temp into linked list
    opr_count++;//operand count +1
    }
    else if(isoperator(input[i])==1){ // if input[i] is an operator
      if(opr_count>=2){ // if operand count >=2
        strcpy(o2,first_opr->data); //copy value into o2
        pop_opr(&first_opr);// destroy value
        strcpy(o1,first_opr->data);//copy value into o1
        pop_opr(&first_opr);//destroy value
        sprintf(temp,"( %s%c %s)",o2,input[i],o1);//copy both num with brackets into temp
        push_opr(temp,&first_opr);//push value
        opr_count--;//operand count -1
      }
    }
  }
  while(first_opr!=NULL){
    strcat(out,first_opr->data); //Concatenate data into out
    pop_opr(&first_opr); //destroy value
  }
  return out; // return out
}
/*
* CONVERT FROM POSTFIX TO INFIX
* reference = https://www.web4college.com/converters/postfix-prefix-to-infix.php;
*
* Variable Description:
*
* temp      = Temporary expression for linked list
* o1,o2     = Temporary operand to sprintf into a string
* opr_count = Numbers of operands
* out       = Output
* first_opr = Head of linked list
* 
* Function Description:
* push_opr = Operands struct pushing
* pop_opr  = Pop an element from linked list (Operands)
*/
char *postfix_infix(char input[],int length){
  opr_ptr first_opr=NULL;
  static char out[MAX]={'\0'};
  memset(out,'\0',MAX);
  char o1[MAX]={'\0'},o2[MAX]={'\0'},temp[MAX]={'\0'};
  int i,opr_count=0;
  for (i=0;i<=length;i++){
    if(isdigit(input[i])){ // single digit
      if(isdigit(input[i+1])){// double digit
        sprintf(temp,"%c%c ",input[i],input[i+1]);//write number into temp
        i++;//i++ since it is checked
      }
      else{
        sprintf(temp,"%c ",input[i]);//write number into temp
      }
      push_opr(temp,&first_opr);//push num into linked list
      opr_count++;//operand count +1
    }
    else if (isoperator(input[i])==1){ //if input[i] is an operator
      if(opr_count>=2){//if operand count >=2
        strcpy(o2,first_opr->data);// copy data to o2
        pop_opr(&first_opr);// pop data
        strcpy(o1,first_opr->data);//copy data to o1
        pop_opr(&first_opr);//pop data
        sprintf(temp,"( %s%c %s)",o1,input[i],o2);//write both num and a ops into temp with brackets
        push_opr(temp,&first_opr);//push temp into linked list
        opr_count--;//oprand count -1;
      }
    }
  }
  while(first_opr!=NULL){
    strcat(out,first_opr->data); //Concatenate data into out
    pop_opr(&first_opr); //pop data
  }
  return out;
}
/*
* CONVERT FROM INFIX TO PREFIX
* reference = https://www.web4college.com/converters/infix-to-postfix-prefix.php;
*
* Variable Description:
*
* temp      = Temporary character for stack pushing
* out       = Output
* first_ops = Head of the linked list
* 
* Function Description:
* strrevv  = String reverse funciton
* push_ops = Operators struct pushing
* pop_ops  = Pop an element from linked list (Operators)
*/
char *infix_prefix(char input[],int length){
  strrevv(input); //string reverse
  ops_ptr first_ops=NULL; 
  static char out[MAX]={'\0'};
  memset(out,'\0',MAX);
  char temp[2];
  int i,j;
  for(i=0,j=0;i<=length;i++){
    if(isdigit(input[i])){ //if num one digit
      if(isdigit(input[i+1])){//if num two digit
        sprintf(out+j,"%c%c ",input[i],input[i+1]);//write number into out
        j+=3;//element counter
        i++;//i++ since it is checked
      }
      else{
      sprintf(out+j,"%c ",input[i]);//write number into out
      j+=2;//element counter
      }
    }
    else if (input[i]==')'){ //if is a closing bracket
      strcpy(temp,")\0"); //copy closing bracket into temp
      push_ops(temp,&first_ops);//push temp into linked list
    }
    else if (input[i]=='('){ 
      while(first_ops!=NULL && first_ops->data!=')'){
        sprintf(out+j,"%c ",first_ops->data);
        pop_ops(&first_ops);
        j+=2;
      }
      pop_ops(&first_ops);
    }
    else if(isoperator(input[i])==1){
      while(first_ops!=NULL && precedence(first_ops->data)>=precedence(input[i]) && first_ops->data!=')'){
        sprintf(out+j,"%c ",first_ops->data);
        pop_ops(&first_ops);
        j+=2;
      }
      temp[0]=input[i];
      temp[1]='\0';
      push_ops(temp,&first_ops);
    }
  }
  while(first_ops!=NULL){
    sprintf(out+j,"%c ",first_ops->data);
    pop_ops(&first_ops);
    j+=2;
  }
  //END OF CONVERSION
  strrevv(out);    
  return out;
}
/*
* CONVERT FROM INFIX TO POSTFIX
* reference = https://www.web4college.com/converters/infix-to-postfix-prefix.php;
*
* Variable Description:
*
* temp      = Temporary character for stack pushing
* out       = Output
* first_ops = Head of the linked list
* 
* Function Description:
* strrevv  = String reverse funciton
* push_ops = Operators struct pushing
* pop_ops  = Pop an element from linked list (Operators)
*/
char *infix_postfix(char input[],int length){
  ops_ptr first_ops=NULL; 
  static char out[MAX]={'\0'};
  memset(out,'\0',MAX);
  char temp[2];
  int i,j=0;
  for (i=0;i<=length;i++){
    if(isdigit(input[i])){
      if(isdigit(input[i+1])){
        sprintf(out+j,"%c%c ",input[i],input[i+1]);
        j+=3;
        i++;
      }
      else{
        sprintf(out+j,"%c ",input[i]);
        j+=2;
      }
    }
    else if(input[i]=='('){
      strcpy(temp,"(\0");
      push_ops(temp,&first_ops);
    }
    else if(input[i]==')'){
      while(first_ops!=NULL && first_ops->data!='('){
        sprintf(out+j,"%c ",first_ops->data);
        pop_ops(&first_ops);
        j+=2;
      }
      pop_ops(&first_ops);
    }
    else if (isoperator(input[i])==1){
      while(first_ops!=NULL && precedence(first_ops->data)>=precedence(input[i]) && first_ops->data!='('){
        sprintf(out + j, "%c ",first_ops->data);
        pop_ops(&first_ops);
        j+=2;
      }
      temp[0]=input[i];
      temp[1]='\0';
      push_ops(temp,&first_ops);
    }
  }
  while(first_ops!=NULL){
      sprintf(out+j,"%c ",first_ops->data);
      pop_ops(&first_ops);
      j+=2;
  }
  return out;
}
/*
* PUSH 
*/
void push_eval(char input[],eval_ptr *first_eval){
  eval_ptr temp_eval=NULL;

  int temp_int,i,j=0;
  char str_digit[5]={'\0'};
  for(i=0;i<=strlen(input);i++){ 
    if(isdigit(input[i])){ //if input is digit, put it in str_digit
      str_digit[j++]=input[i];
    }
  }
  temp_int=atoi(str_digit); //convert str_digit to temporary integer
  if(first_eval==NULL){ //if head of the pointer is NULL
    (*first_eval)=(evaluation*)malloc(sizeof(struct Evaluation)); //allocate space
    (*first_eval)->value=temp_int;//pointer points to temporary integer
    (*first_eval)->next=NULL;//pointer next points to NULL since there's no element 
  }
  else{//if head of pointer contains element
    temp_eval=(*first_eval); //temporary pointers = first
    *first_eval=(evaluation*)malloc(sizeof(struct Evaluation)); //allocate space
    (*first_eval)->value=temp_int;//head of value points to temp num
    (*first_eval)->next=temp_eval;//head points temp as it's next element
  }
}
void push_opr(char input[],opr_ptr *first_opr){
  opr_ptr temp_opr;
    temp_opr=(operands*)malloc(sizeof(struct Operands));
    strcpy(temp_opr->data,input);
    if(first_opr==NULL){
      temp_opr->next=NULL;
    }
    else{
      temp_opr->next=*first_opr;
    }
    (*first_opr)=temp_opr;
} 
void push_ops(char input[],ops_ptr *first_ops){
  ops_ptr temp_ops;
    temp_ops=(operators*)malloc(sizeof(struct Operators));
    temp_ops->data=input[0];
    if(first_ops==NULL){
      temp_ops->next=NULL;
    }
    else{
      temp_ops->next=*first_ops;
    }
    (*first_ops)=temp_ops;
}

void pop_eval(eval_ptr *first_eval){
  eval_ptr temp_eval=NULL;

  if((*first_eval)->next==NULL){
    *first_eval=NULL;
  }
  else{
    temp_eval=*first_eval;
    *first_eval=temp_eval->next;
    free(temp_eval);
  }
}
void pop_opr(opr_ptr *first_opr){
  opr_ptr temp_opr = malloc(sizeof(struct Operands));
  if(first_opr!=NULL){
    temp_opr=*first_opr;
    *first_opr=temp_opr->next;
    free(temp_opr);
  }
}
/*
* POP
*/
void pop_ops(ops_ptr *first_ops){
  ops_ptr temp_ops = malloc(sizeof(struct Operators));
  /*if((*first_ops)->next==NULL){
    first_ops=NULL;
  }
  else{*/
    temp_ops=*first_ops;
    *first_ops=(*first_ops)->next;
    free(temp_ops);
  //}
}
/*
* PRECEDENCE
*/

int precedence(char input){
  switch(input){
    case '(':
    return 1;
    break;
    case '+':
    case '-':
    return 2;
    break;
    case '/':
    case '*':
    return 3;
    break;
    case '^':
    return 4;
    break;
  }
  return 0;
}
/*
* DISPLAY ERROR: case 1
*/

void displayerror(int location,char input[],int type){
  switch(type){
    case 1:
    printf("%s",input);
    for(int i=0;i<=location;i++){
      if(i==location){
        printf("^\n");
      }
      else{
        printf(" ");
      }
    }
    break;
    case 2:
    printf("\n%s",input);
    for(int i=0;i<=location;i++){
      if(input[i]=='(' || input[i]==')'){
        location+=3;
        continue;
      }
      if(i==location){
        printf("^\n");
        break;
      }
    printf(" ");
      
    }
    break;
  }
}
/*
* Determining the expression type
*/
int extype(char input[]){
  if(isoperator(input[0])){
    return 1;
  }
  if(isoperator(input[strlen(input)-2])){
    return 2;
  }
  return 0;
}

int isoperator(char input){
  if(input=='+' || input=='/' || input=='*' || input=='^' || input=='-'){
    return 1;
  }
  else{
  return 0;
  }
}

char *strrevv(char *str){
  char *ptr1, *ptr2;

  if (! str || ! *str)
      return str;
  for (ptr1 = str, ptr2 = str + strlen(str) - 1; ptr2 > ptr1; ++ptr1, --ptr2){
    *ptr1^=*ptr2;
    *ptr2^=*ptr1;
    *ptr1^=*ptr2;
  }
  return str;
}

int pows (int x,int n){
    int i; /* Variable used in loop counter */
    int number = 1;

    for (i = 0; i < n; ++i)
        number *= x;

    return(number);
}