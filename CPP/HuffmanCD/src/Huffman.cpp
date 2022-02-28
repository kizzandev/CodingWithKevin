#include <bits/stdc++.h>
#include "biblioteca/funciones/arrays.hpp"
#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/lists.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/Array.hpp"
#include "biblioteca/tads/BitFileReader.hpp"
#include "biblioteca/tads/BitFileWriter.hpp"
#include "biblioteca/tads/Coll.hpp"
#include "biblioteca/tads/List.hpp"
#include "biblioteca/tads/Map.hpp"
#include "biblioteca/tads/Queue.hpp"
#include "biblioteca/tads/Stack.hpp"
#include "biblioteca/tads/huffman/HuffmanSetup.hpp"
#include "biblioteca/tads/huffman/Progress.hpp"

struct InfoByte
{
   unsigned int n; // cantidad de veces que se repite cada byte
   string cod;     // codigo
} arr[256];        // inicializo arr

// declaracion de funciones
void comprimir(string nomArch);
void paso1C(string nomArch);
int cmpHT(HuffmanTreeInfo a, HuffmanTreeInfo b);
List<HuffmanTreeInfo> paso2C();
HuffmanTreeInfo *paso3C(List<HuffmanTreeInfo> lst);
void paso4C(HuffmanTreeInfo *raiz);
void paso5C(string nomArch);
void descomprimir(string nomArchCompr);
void paso1D(FILE *f, HuffmanTreeInfo *&raiz);
void paso2D(FILE *f, HuffmanTreeInfo *raiz, string nomArchCompr);
void progressMade();

// paso 1 - contar cuantas veces aparece cada byte
void paso1C(string nomArch)
{
   FILE *f = fopen(nomArch.c_str(), "r+b"); // abro archivo
   unsigned char c = read<char>(f);         // leo caracter
   while (!feof(f))
   {
      arr[c].n++;        // incrementar freq del caracter correspondiente
      c = read<char>(f); // proximo caracter
   }
   fclose(f); // cierro archivo
}

int cmpHT(HuffmanTreeInfo a, HuffmanTreeInfo b)
{
   if (a.n - b.n == 0)
   {
      return a.c - b.c;
   }
   return a.n - b.n;
}

// paso 2 - crear la lista y ordenarla
List<HuffmanTreeInfo> paso2C()
{
   List<HuffmanTreeInfo> lista = newList<HuffmanTreeInfo>();
   for (int i = 0; i < 256; i++)
   {
      if (arr[i].n != 0)
      {
         HuffmanTreeInfo *nodo = huffmanTreeInfo(i, arr[i].n, NULL, NULL);
         listOrderedInsert<HuffmanTreeInfo>(lista, *nodo, cmpHT);
      }
   }
   return lista;
}

// paso 3 - convertir la lista en arbol
HuffmanTreeInfo *paso3C(List<HuffmanTreeInfo> lst)
{
   // unsigned int i = 256;
   unsigned int i = 0;

   HuffmanTreeInfo *nL = listRemoveFirstPtr<HuffmanTreeInfo>(lst);
   while (!listIsEmpty(lst))
   {
      HuffmanTreeInfo *nR = listRemoveFirstPtr<HuffmanTreeInfo>(lst);
      HuffmanTreeInfo *nT = huffmanTreeInfo(i, nL->n + nR->n, nL, nR);
      listOrderedInsert<HuffmanTreeInfo>(lst, *nT, cmpHT);
      i++;
      nL = listRemoveFirstPtr<HuffmanTreeInfo>(lst);
   }
   return nL;
}

// paso 4 - recorrer el arbol y guardar los codigos en el array
void paso4C(HuffmanTreeInfo *raiz)
{
   HuffmanTree ht = huffmanTree(raiz);
   while (huffmanTreeHasNext(ht))
   {
      string cod;
      HuffmanTreeInfo *htI = huffmanTreeNext(ht, cod);
      unsigned int i = htI->c;
      if (i < 256)
      {
         arr[i].cod = cod;
      }
   }
}

// paso 5 - recorrer el archivo original, grabar encab y bit x bit el archivo comprimido
void paso5C(string nomArch)
{
   string s = nomArch + ".huf";
   FILE *huf = fopen(s.c_str(), "w+b");

   // cantidad de hojas
   unsigned char cCar = 0;
   for (int i = 0; i < 256; i++)
   {
      if (arr[i].n != 0)
      {
         cCar++;
      }
   }
   write<unsigned char>(huf, cCar); // cantidad de hojas

   // t Registros
   for (int i = 0; i < 256; i++)
   {
      if (arr[i].n != 0)
      {
         write<char>(huf, i);                  // caracter c
         write<char>(huf, length(arr[i].cod)); // longitud del codigo
         for (int j = 0; j < length(arr[i].cod); j++)
         {
            char c = arr[i].cod[j];
            write<char>(huf, c); // codigo de c
         }
      }
   }

   FILE *f = fopen(nomArch.c_str(), "r+b");
   // longitud del archivo
   // = 0
   unsigned int archLong = 0;
   archLong = fileSize<char>(f);
   write<unsigned int>(huf, archLong); // longitud del archivo
   seek<char>(f, 0);
   // info codificada
   unsigned char c = read<char>(f);
   BitWriter bw = bitWriter(huf); // informacion codificada
   while (!feof(f))
   {
      string cod = "";
      cod = arr[c].cod;

      for (int j = 0; j < length(cod); j++)
      {
         char b = cod[j];
         bitWriterWrite(bw, charToInt(b));
      }
      c = read<char>(f);
   }
   bitWriterFlush(bw);

   fclose(f);
   fclose(huf);
}

void comprimir(string nomArch)
{
   // paso 1 - contar cuantas veces aparece cada byte
   paso1C(nomArch);

   // paso 2 - crear la lista y ordenarla
   List<HuffmanTreeInfo> lista = paso2C();

   // paso 3 - convertir la lista en arbol
   HuffmanTreeInfo *raiz = paso3C(lista);

   // paso 4 - recorrer el arbol y guardar los codigos en el array
   paso4C(raiz);

   // paso 5 - recorrer el archivo original y grabar bit x bit el archivo comprimido
   paso5C(nomArch);
}

// paso 1 - reconstruir el arbol
void paso1D(FILE *f, HuffmanTreeInfo *&raiz)
{
   raiz = huffmanTreeInfo(257, 0, NULL, NULL); // creo la raiz
   unsigned char hojas = read<unsigned char>(f);
   char k;
   for (int i = 0; i < hojas; i++)
   {
      HuffmanTreeInfo *aux = raiz;
      char c = read<char>(f);
      char lon = read<char>(f);
      while (lon > 1)
      {
         k = read<char>(f);
         if (k == 48) // si es el caracter '0'
         {
            if (aux->left != NULL)
            {
               aux = aux->left;
            }
            else
            {
               aux->left = huffmanTreeInfo(257, 0, NULL, NULL);
               aux = aux->left;
            }
         }
         if (k == 49) // si es el caracter '1'
         {
            if (aux->right != NULL)
            {
               aux = aux->right;
            }
            else
            {
               aux->right = huffmanTreeInfo(257, 0, NULL, NULL);
               aux = aux->right;
            }
         }
         lon--;
      }
      k = read<char>(f);
      if (k == 48)
      {
         aux->left = huffmanTreeInfo(c, 0, NULL, NULL);
      }
      if (k == 49)
      {
         aux->right = huffmanTreeInfo(c, 0, NULL, NULL);
      }
   }
}

// paso 2 - leer informacion codifica y decodificar en un archivo nuevo
void paso2D(FILE *f, HuffmanTreeInfo *raiz, string nomArchCompr)
{
   unsigned int lonArch = read<unsigned int>(f);

   string s = nomArchCompr;
   int h = lastIndexOf(s, '.');
   s = substring(s, 0, h);                // elimino el ".huf"
   FILE *fdesc = fopen(s.c_str(), "w+b"); // creo el archivo

   HuffmanTreeInfo *aux = raiz; // trabajo con un arbol auxiliar
   BitReader br = bitReader(f);

   int i = bitReaderRead(br);
   while (lonArch > 0)
   {
      while (aux->c == 257) // 257 significa que no es una hoja
      {
         if (i == 0)
         {
            aux = aux->left;
         }
         if (i == 1)
         {
            aux = aux->right;
         }
         i = bitReaderRead(br);
      }
      write<char>(fdesc, aux->c); // escribo el caracter en el archivo
      aux = raiz;                 // vuelvo a la raiz
      lonArch--;
   }
   fclose(fdesc); // cierro el archivo
}

void descomprimir(string nomArchCompr)
{
   HuffmanTreeInfo *raiz = NULL;
   FILE *f = fopen(nomArchCompr.c_str(), "r+b");

   // paso 1 - reconstruir el arbol
   paso1D(f, raiz);

   // paso 2 - leer informacion codifica y decodificar en un archivo nuevo
   paso2D(f, raiz, nomArchCompr);

   fclose(f);
}

// int main(int argc, char **argv)
int main()
{
   // nombre del archivo con el que voy a trabajar
   string nomArch = "";
   // nomArch = stringToString(argv[1]);
   cin >> nomArch;

   if (!endsWith(nomArch, ".huf")) // comprimir
   {
      comprimir(nomArch);
      progressMade(); // "progreso de mentira"
   }
   else // descomprimir
   {
      descomprimir(nomArch);
      progressMade(); // "progreso de mentira 2"
   }

   return 0;
}

void progressMade()
{
   Progress p1 = progress("Progress: ");
   progressInit(p1);
   for (int i = 0; i < 100; i++)
   {
      progressShow(p1, i);
      _heavyProcess(500000);
   }
   progressDestroy(p1);

   cout << endl;
}
