//GRID INVADERS

/*Universidade Federal de Goias
  Instituto de Informática
  Professor Doutor Thierson Couto Rosa
  Alunos: Matheus Melo da Costa
          Matheus Vinicius Dias Ribeiro
*/

//INSTRUÇÕES
/*
1 - O jogo só pode ser fechado pelo menu, por questões de segurança ao save-game;
2 - Para entrar no menu, aperte [P]; Para sair do jogo, aperte [ESQ] no menu;
3 - Setas esq. e dir. = movem a nave;
4 - Espaço = atira;
*/

#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define resx 1280
#define resy 720

//---------------------------------------------
//STRUCTS;
typedef struct coordenadas{
   int x, y, z;
}dms;

//---------------------------------------------
//FUNÇÕES GERAIS;
void iniciar();
void zerarbuffer();
void desenha(BITMAP *buffer);
void ctrlespaco(int *vel_espaco, int posicao_espaco);
void ctrltiro(int *aux, dms *tiro, dms nave, SAMPLE *som);
void ctrlinimigo(BITMAP *buffer, BITMAP *alien, BITMAP *alienmau, BITMAP *heart, int vel_inimigo, dms *inimigo, dms *tiro, int *loop, int *qualfase, FILE *save);
void menu(BITMAP *buffer, BITMAP *espaco, int *menuaux, int *entramenu);
void geradordeinimigo(dms *gd_inimigo);
void zeraconferemorte(int *conferemorte);
void zeralimit(int limit[3][10]);
//---------------------------------------------
//VARIÁVEIS GLOBAIS;
int pressenter_soma = 0, inimigos_soma = 0, desenhainimigo = 0, limit[3][10], score = 0, coracao = 5, condicaoextra = 0;
int vel2 = 2;

//---------------------------------------------
//funções de temporizador;
void tempopressenter() {pressenter_soma++;}  
void tempoinimigos() {inimigos_soma++;}
//---------------------------------------------

//PROGRAMA PRINCIPAL;
int main() {
    iniciar(); //Iniciadores da ALLEGRO;
    
    // BITMAPS E BUFFERS;
    BITMAP *buffer = create_bitmap(resx, resy); //Ponteiro do tipo BITMAP para gerar um sprite na tela;
    BITMAP *espaco = load_bitmap("img/espaco.bmp", NULL);      //Espaço;
    BITMAP *imagemesq = load_bitmap("img/navesesq.bmp", NULL); //Nave deitada esquerda;
    BITMAP *imagemdir = load_bitmap("img/navesdir.bmp", NULL); //Nave deitada direita;
    BITMAP *imagemorg = load_bitmap("img/naves.bmp", NULL);    //Nave posição original;
    BITMAP *principal = load_bitmap("img/naves.bmp", NULL);    //Buffer da nave;
    BITMAP *missil = load_bitmap("img/tiro.bmp", NULL);        // Missil;
    BITMAP *satelite = load_bitmap("img/satelite.bmp", NULL);  // Satelite;
    BITMAP *alien = load_bitmap("img/alien.bmp", NULL);    //Inimigo;
    BITMAP *alienmau = load_bitmap("img/alienmau.bmp", NULL);    //Inimigo;
    BITMAP *heart = load_bitmap("img/heart.bmp", NULL);    //vidas;
    BITMAP *pts = load_bitmap("img/pts.bmp", NULL);    //pontos;
    BITMAP *over = load_bitmap("img/gameover.bmp", NULL);    //pontos;
    BITMAP *youwin = load_bitmap("img/youwin.bmp", NULL);    //pontos;
    
    //SAVE GAME DO TIPO FILE
    FILE *save;
    save = fopen("pontos.txt", "r");      //Lendo o save-game;
    
    //SONS
    SAMPLE *tirosom = load_sample("sons/laser.wav");
    SAMPLE *menusom = load_sample("sons/menu.wav");
     play_sample(menusom, 255, 128, 1000, 1);
    //VARIÁVEIS 
    dms nave, tiro, inimigo1[10];                                  //Coordenadas;
    int vel_nave = 2, posicao_espaco = 0, vel_espaco = 1, vel_inimigo = 6;  //Velocidades;
    int aux = 0, saijogo = 0, entramenu = 0, qualfase = 1, loopimg = 0, navegameover = 0;    //Todos os auxiliares; 
    nave.x = 590; //Posição x inicial da nave;
    nave.y = 600; //Posição y inicial da nave;
    
    //GERANDO VETOR DE INIMIGOS ALEARTÓRIOS;
    srand(time(NULL));
    geradordeinimigo(inimigo1); 
    
    //TEMPORIZADORES;
    install_int_ex(tempopressenter, SECS_TO_TIMER(1)); //Velocidade da aparição pressenter;
    install_int_ex(tempoinimigos, SECS_TO_TIMER(1)); //Define que a cada segundo o tmp do alien incrementa;
    
    fscanf(save, "%i %i", &qualfase, &score);
    //ONDE A MÁGICA ACONTECE;
	while (saijogo == 0) { 

          //Menu;
          if (key[KEY_P]) entramenu = 0;
          if (entramenu == 0) menu(buffer, espaco, &saijogo, &entramenu);
          
          //Controlador do cenário;
          ctrlespaco(&posicao_espaco, vel_espaco);
          
          //DEFINIÇÃO DE TECLAS INIT;
          //nave;
          if(key[KEY_RIGHT]) {nave.x += vel_nave; principal = imagemdir;}                
          if(key[KEY_LEFT]) {nave.x -= vel_nave;  principal = imagemesq;}
          if (nave.x > 1180) nave.x = 1180;
          if (nave.x < 0) nave.x = 0;  
          
          //tiro;
          if(key[KEY_SPACE]) {aux = 1; }
          ctrltiro(&aux, &tiro, nave, tirosom);
          
          //DEFINIÇÃO DE TECLAS END;
          
          
          //DESENHAR NO BUFFER E TELA; 
          // joga as imagens no buffer;
          draw_sprite(buffer, espaco, 0, posicao_espaco);
          draw_sprite(buffer, espaco, 0, posicao_espaco-resy);
          draw_sprite(buffer, pts, 560, 5);
          textprintf_ex(buffer, font, 700, 15, makecol(255, 255, 255), -1, "%d", score);
          if (qualfase == 1) {
             vel_inimigo = 3;
             vel2 = 1;
             vel_nave = 2;
             vel_espaco = 1;
             draw_sprite(buffer, satelite, 0, 445);    
             ctrlinimigo(buffer, alien, alienmau, heart, vel_inimigo, inimigo1, &tiro, &loopimg, &qualfase, save);
          }
          if (qualfase == 2) {
             vel_inimigo = 2;
             vel_espaco = 2;
             vel2 = 1;
             vel_nave = 3;
             draw_sprite(buffer, satelite, 0, 445);    
             ctrlinimigo(buffer, alien, alienmau, heart, vel_inimigo, inimigo1, &tiro, &loopimg, &qualfase, save);
          }
          if (qualfase == 3) {
             vel_inimigo = 1;
             vel_espaco = 2;
             vel2 = 2;
             vel_nave = 4;
             draw_sprite(buffer, satelite, 0, 445);    
             ctrlinimigo(buffer, alien, alienmau, heart, vel_inimigo, inimigo1, &tiro, &loopimg, &qualfase, save);
          }
          if (qualfase == 4) { 
             if (inimigos_soma < 3) {
                navegameover = 1;
                draw_sprite(buffer, over, 0, 200);
             }  
             else { 
                  qualfase = 1; 
                  inimigos_soma = 0;
                  navegameover = 0;
                  score = 0;
                  entramenu = 0;
                  save = fopen("pontos.txt", "w");
                  fprintf(save, "1 0");
             }          
          }
          if (qualfase == 5) { 
             if (inimigos_soma < 3) {
                navegameover = 1;
                draw_sprite(buffer, youwin, 0, 200);
             }  
             else { 
                  qualfase = 1; 
                  inimigos_soma = 0;
                  navegameover = 0;
                  score = 0;
                  entramenu = 0;
                  save = fopen("pontos.txt", "w");
                  fprintf(save, "1 0");
             }          
          }
          if (navegameover == 0) draw_sprite(buffer, missil, 0+tiro.x, 0+tiro.y); 
          if (navegameover == 0) draw_sprite(buffer, principal, 0+nave.x, 0+nave.y); 
          //desenha tudo na tela e limpa depois;     
          desenha(buffer);
          principal = imagemorg;
	}
	
    // Destroi todos os bitmaps;
    destroy_bitmap(buffer);
    destroy_bitmap(imagemesq); 
    destroy_bitmap(principal);
    destroy_bitmap(imagemdir);
    destroy_bitmap(imagemorg);
    destroy_bitmap(espaco);
    destroy_bitmap(missil);
    destroy_bitmap(alien);
    destroy_bitmap(alienmau);
    destroy_bitmap(heart);
    destroy_bitmap(pts);
    destroy_bitmap(over);
    destroy_bitmap(youwin);
    destroy_sample(tirosom);
    destroy_sample(menusom);
    fclose(save);
    zerarbuffer();
	return 0;
}
END_OF_MAIN() 

//---------------------------------------------
//---------------------------------------------

void zeralimit(int limit[3][10]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 10; j++) {
              limit[i][j] = 0;  
        } 
    }     
}
//---------------------------------------------
//---------------------------------------------

void menu(BITMAP *buffer, BITMAP *espaco, int *saijogo, int*entramenu) {
     //Aloca as imagens do menu apenas no menu para economizar memória;
     BITMAP *logo = load_bitmap("img/logo.bmp", NULL);
     BITMAP *pressenter = load_bitmap("img/pressenter.bmp", NULL);
     
     int y = 0, delaypause; //Velocidade especifica do espaço para o menu;
     delaypause = inimigos_soma;
     pressenter_soma = 0;
     
     while (*entramenu == 0) {
           ctrlespaco(&y, 1);
           draw_sprite(buffer, espaco, 0, y);
           draw_sprite(buffer, espaco, 0, y-resy);
           draw_sprite(buffer, logo, 0, 0); 
           
           //Intervalo pressenter;
           if (pressenter_soma < 1) draw_sprite(buffer, pressenter, 0, 370);
           if (pressenter_soma == 2) pressenter_soma = 0;
           
           //CRIADORES :) 
          textprintf_ex(buffer, font, 585, 690, makecol(255,255,255), -1, "Developed by:");
          textprintf_ex(buffer, font, 515, 705, makecol(255,255,255), -1, "Matheus Melo & Matheus Vinicius");
          
          desenha(buffer);
           //Saida do menu ou do jogo;
           if (key[KEY_ENTER]) *entramenu = 1;                
           if (key[KEY_ESC]) {*saijogo = 1; break;}               
     }
    inimigos_soma = delaypause;
    destroy_bitmap(logo);
    destroy_bitmap(pressenter);
}
//---------------------------------------------
//---------------------------------------------
void ctrlespaco(int *posicao_espaco, int vel_espaco) {
     if (*posicao_espaco > 720) *posicao_espaco = 0;
     *posicao_espaco += vel_espaco;     //Velocidade do cenário;
}
//---------------------------------------------
//---------------------------------------------
void ctrltiro(int *aux, dms *tiro, dms nave, SAMPLE *som) {
     if (*aux) { 
        tiro->y -= 3;
        if (tiro->y == nave.y+41) play_sample(som, 255, 128, 1000, 0); 
     }
     if (!(*aux)) {
        tiro->x = nave.x+44;
        tiro->y = nave.y+44;        
     }
     if(tiro->y < -13) *aux = 0;
}
//---------------------------------------------
//---------------------------------------------
void geradordeinimigo(dms *gd_inimigo){
     
     int i, j, auxinimigo = 0;
    for (i = 0; i < 10; i++) {
        gd_inimigo[i].z = rand()%10;   //Gera valores aleartórios para os inimigos;
        for (j = 0; j < i; j++) {
              if (gd_inimigo[j].z == gd_inimigo[i].z) auxinimigo++;
              if (auxinimigo == 1) {i--; auxinimigo = 0; break;}  
        }    
    }     
    
    //Posiciona na tela o inimigo de acordo com o sorteamento;
    for (i = 0; i < 10; i++) {
        switch(gd_inimigo[i].z) {
             case 0: 
                  gd_inimigo[i].x = 35; 
                  gd_inimigo[i].y = -100;
                  gd_inimigo[i].z = 530;
                  break;
             case 1: 
                  gd_inimigo[i].x = 140; 
                  gd_inimigo[i].y = -100;
                  gd_inimigo[i].z = 530;
                  break;
             case 2: 
                  gd_inimigo[i].x = 244; 
                  gd_inimigo[i].y = -100;
                  gd_inimigo[i].z = 530;
                  break;
             case 3: 
                  gd_inimigo[i].x = 350; 
                  gd_inimigo[i].y = -100;
                  gd_inimigo[i].z = 530;
                  break;
             case 4: 
                  gd_inimigo[i].x = 454;
                  gd_inimigo[i].y = -100;
                  gd_inimigo[i].z = 530;
                  break;
             case 5: 
                  gd_inimigo[i].x = 729; 
                  gd_inimigo[i].y = -100;
                  gd_inimigo[i].z = 530;
                  break;
             case 6: 
                  gd_inimigo[i].x = 833; 
                  gd_inimigo[i].y = -100;
                  gd_inimigo[i].z = 530;
                  break;
             case 7: 
                  gd_inimigo[i].x = 937; 
                  gd_inimigo[i].y = -100;
                  gd_inimigo[i].z = 530;
                  break;
             case 8: 
                  gd_inimigo[i].x = 1042;
                  gd_inimigo[i].y = -100; 
                  gd_inimigo[i].z = 530;
                  break;
             case 9: 
                  gd_inimigo[i].x = 1147;
                  gd_inimigo[i].y = -100; 
                  gd_inimigo[i].z = 530;
                  break;                     
        } 
    }
}
//---------------------------------------------
//---------------------------------------------
void ctrlinimigo(BITMAP *buffer, BITMAP *alien, BITMAP *alienmau, BITMAP *heart, int vel_inimigo, dms *inimigo, dms *tiro, int *loop, int *qualfase, FILE *save) { 
     int i, m = 1;  

     //Conferir se o tiro colidiu com o alien;
     for (i = 0; i < 10; i++) {
         if (tiro->x+10 > inimigo[i].x && tiro->x < inimigo[i].x+69 && tiro->y+10 > inimigo[i].y && tiro->y < inimigo[i].y+36) { 
           desenhainimigo++; 
           condicaoextra = desenhainimigo + condicaoextra; 
           inimigo[i].y = -100;
           if (limit[*loop][i] != 0) {}
           else { limit[*loop][i] = 1; inimigo[i].z =  inimigo[i].z - 40; }    
           tiro->y = -150;
           score = score + 5;
         }
         
        //Conferir se o inimigo chegou ao satélite;  
        if(inimigo[i].y == inimigo[i].z) {
           if (limit[*loop][i] != 0 ) {}
           else { limit[*loop][i] = 2; inimigo[i].z =  inimigo[i].z - 40; }   
           inimigo[i].y = -100; 
           desenhainimigo++;
           condicaoextra = desenhainimigo + condicaoextra; 
           coracao = coracao - 1;
        }
     }
     
     // Printar apenas os inimigos vindo de cima;
     for (i = 0; i < 10; i++) {
         if (inimigos_soma > vel_inimigo*(i+1) && desenhainimigo <= i) { draw_sprite(buffer, alien, inimigo[i].x+20, inimigo[i].y); inimigo[i].y+=vel2; } 
     }

        for (m = 0; m <= *loop; m++) {
            for (i = 0; i < 10; i++) {
                if(limit[m][i] == 2 && m == 0) draw_sprite(buffer, alienmau, inimigo[i].x+20, 535);
                if(limit[m][i] == 2 && m == 1) draw_sprite(buffer, alienmau, inimigo[i].x+20, 495);
                if(limit[m][i] == 2 && m == 2) draw_sprite(buffer, alienmau, inimigo[i].x+20, 455);
            }            
        } 
        for (m = 0; m <= *loop; m++) {
            for (i = 0; i < 10; i++) {
            if(limit[m][i] == 1 && m == 0) draw_sprite(buffer, alien, inimigo[i].x+20, 535);
            if(limit[m][i] == 1 && m == 1) draw_sprite(buffer, alien, inimigo[i].x+20, 495);
            if(limit[m][i] == 1 && m == 2) draw_sprite(buffer, alien, inimigo[i].x+20, 455);  
                     
            } 
        }
        
        //Printar coracoes
        if (coracao >= 1){ draw_sprite(buffer, heart, 5, 5); }
        if (coracao >= 2){ draw_sprite(buffer, heart, 30, 5); }
        if (coracao >= 3){ draw_sprite(buffer, heart, 55, 5); }
        if (coracao >= 4){ draw_sprite(buffer, heart, 80, 5); }
        if (coracao == 5){ draw_sprite(buffer, heart, 105, 5); }
        if (coracao == 0){ 
           *qualfase = 4; 
           save = fopen("pontos.txt", "w");
           fprintf(save, "1 0");
           inimigos_soma = 0;  
           desenhainimigo = 0;  
           coracao = 5; 
           zeralimit(limit);  
           geradordeinimigo(inimigo);
           *loop = 0;    
        }
        //Identificador de "Linha" e fase atual;
        if (desenhainimigo == 10) {
           if (*loop == 0) {*loop = 1;}
           else if (*loop == 1) *loop = 2;
           else {
                *qualfase = *qualfase+1; 
                *loop = 0; 
                zeralimit(limit); 
                geradordeinimigo(inimigo); 
                coracao = 5;
                save = fopen("pontos.txt", "w");
                fprintf(save, "%i %i", *qualfase, score);
                if (*qualfase == 4) {fprintf(save, "1 0"); *qualfase = 5;}
           }
           desenhainimigo = 0;
           inimigos_soma = 0; 
      }
}
//---------------------------------------------
void desenha(BITMAP *buffer) { 
     draw_sprite(screen, buffer, 0, 0); //desenha o buffer na tela;                                         
     clear(buffer); //limpa o buffer;
     rest(0);   
}
//---------------------------------------------
//---------------------------------------------
void iniciar() {
	int depth, res;
	allegro_init(); //inicia a biblioteca;
	depth = desktop_color_depth(); 
	if (depth == 0) depth = 32;
	set_color_depth(depth); //define o tipo de cor;
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, resx, resy, 0, 0); //resolução e tipo de tela;
	if (res != 0) {
		allegro_message(allegro_error); //mensagem para possíveis erros;
		exit(-1);
	}
    set_window_title("Grid Invasors");
	install_timer(); //instala temporizador do jogo;
	install_keyboard(); //instala o teclado;
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
}
//---------------------------------------------
//---------------------------------------------
void zerarbuffer() {
	clear_keybuf(); //limpa o buffer do teclado;
}
