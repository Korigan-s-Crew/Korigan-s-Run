#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "example.h"

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *image = NULL;
    int statut = EXIT_FAILURE;
    // Crée la couleur blanche avec rgb et alpha
    SDL_Color blanc = {255, 255, 255, 255};

    // Initialise la fenêtre et le rendu
    if(0 != init(&window, &renderer, 1200, 800))
            goto Quit;
    // Crée une texture à partir d'une image BMP
    image = loadImage("example.bmp", renderer);
    if(NULL == image)
        goto Quit;
    
    // Remplit la fenêtre de blanc
    setWindowColor(renderer, blanc);

    // Crée un rectangle de destination pour la texture
    SDL_Rect dst = {0, 0, 0, 0};
    // Récupère les dimensions de la texture
    SDL_QueryTexture(image, NULL, NULL, &dst.w, &dst.h);
    // Positionne l'image par rapport au rectangle crée au-dessus 
    SDL_RenderCopy(renderer, image, NULL, &dst);
    statut = EXIT_SUCCESS;
    // Affiche le rendu
    SDL_RenderPresent(renderer);
    // Attend 6 secondes
    SDL_Delay(6000);

    Quit:
    if(NULL != image)
        SDL_DestroyTexture(image);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    // Crée la fenêtre et le rendu
    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_RESIZABLE, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL; 
    SDL_Texture *texture = NULL;
    // Charge l'image dans une surface temporaire
    tmp = SDL_LoadBMP(path);
    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    // Crée une texture à partir de la surface temporaire
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    // Libère la surface temporaire
    SDL_FreeSurface(tmp);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    // Retourne la texture
    return texture;
}


int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    // Change la couleur du pinceau utiliser (un peu comme sur paint tu change la couleur du pinceau avant de dessiner)
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    // Remplit toute la fenêtre de la couleur du pinceau
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;  
}
