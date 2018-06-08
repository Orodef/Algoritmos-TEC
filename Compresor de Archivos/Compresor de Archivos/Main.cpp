//huffman.cpp
//Huffman Encoding (Data Compression)
//Compiler used: Dev-C++ 4.9.9.2
//FB - 201011301

#include<iostream>
#include "stdafx.h"
#include<fstream>
#include <iostream>
#include<string>
#include<iomanip> //for width()
#include<cctype> //for sprintf()

#pragma warning (disable: 4996)

#define HELP_ERROR 99
#define width_unit 5

using namespace std;

// (Templated d-heap) (on dynamic array of pointers)
// priority queue
// min (root=min) ((balanced) d-tree on dynamic array) d-heap
template<class T>
class Queue
{
public:

	Queue(int d = 2); //constructor de clase
	~Queue(void); //destructor de clase
	void enq(T*); //poner en la cola
	T* deq(void); //sacar de la cola
	T* front(void); //el elemento de en frente (top)
	bool empty(void) const; //verificaci[on de si está vacío
	bool full(void) const; //verificaci[on de si está lleno

private:

	int back; //el último elemento en la cola
	T* *arr; //arreglp dinámico
	int size; //tamaño actual del arreglo
	static const int SIZE = 10; //tamaño en el que incrementa
	int D; //mayor npumero de hijos en un nodo padre
		   
	Queue(const Queue &);
	const Queue & operator=(const Queue &);

	//Funciones de utilidad para arreglar
	//cuando se agregan o se eliminan elementos
	void reheapup(int, int); //corregir hacia arriba
	void reheapdown(int, int); //corregir hacia abajo
	void swap(T* &, T* &); //funcion de intercambio que es necesaria para las dos anteriores

}; //termina la parte de lña clase


   // constructor (crea una nueva cola) 
template<class T>
Queue<T>::Queue(int d)
{
	if (d<2) d = 2; //soporta como mínimo 2
	D = d;
	back = 0;
	size = SIZE;
	arr = new T*[size];
}

// pregunta si la cola está vacía
template<class T>
bool Queue<T>::empty(void) const
{
	return (back <= 0);
}

// pregunta si la cola está llena
template<class T>
bool Queue<T>::full(void) const
{
	return (back >= size);
}

// pregunta por el elemento de en frente de la cola
template<class T>
T* Queue<T>::deq(void)
{
	if (empty())
	{
		cerr << "deq error! exiting..." << endl;
		exit(1);
	}

	T* rval = arr[0];
	arr[0] = arr[back - 1]; //el elmento de atrás se mueve al frente
	--back;
	reheapdown(0, back - 1); //se llama a la función para que corrija el orden de nuevo 
	return rval;
}

// SE retorna una copia del primer elemento pero la cola no cambia
template<class T>
T* Queue<T>::front(void)
{
	if (empty())
	{
		cerr << "deq error! exiting..." << endl;
		exit(1);
	}

	return arr[0];
}

// se añanade un nuevo elemento a la cola
template<class T>
void Queue<T>::enq(T* foo)
{
	if (full()) //Si el arreglo está lleno entonces se aumenta su tamaño
	{
		int nsize = size + SIZE; //el tamaño del nuevo arreglo
		T* *narr = new T*[nsize]; //el nuevo arreglo como tal
		for (int i = 0; i<size; ++i) //se copia el arreglo anterior al nuevo
			narr[i] = arr[i];
		delete[] arr; //se elimina la memoria que estaba ocupando el arreglo anterior
		arr = narr; //se actualiza el puntero
		size = nsize; //se actualiza el tamaño
	}

	//se añade el nuevo elemento a la parte trasera del arreglo
	//se allama a la función reheapup para que corrija el orden
	arr[back++] = foo; //arr[back]=foo;++back;
	reheapup(0, back - 1);
}

// Esta es la función recursiva que se encarga de corregir el orden en el arreglo
// desde la parte de abajo después de haber añadido un elemento a la parte trasera
template<class T>
void Queue<T>::reheapup(int root, int bottom)
{
	int parent; // nodo padre del último elemento

	if (bottom > root)
	{
		parent = (bottom - 1) / D;

		//compara dos nodos y si el orden es iincorrecto los intercambia
		//y hace una llamada recursiva para así ir subiendo en el arbol
		//hasta que todo el orden del arbol sea el correcto
		if (*arr[parent] > *arr[bottom])
		{
			swap(arr[parent], arr[bottom]);
			reheapup(root, parent);
		}
	}
}

// Esta es la función recursiva que se encarga del orden de la cola
// de arriba hacia abajo cuando un elemen se agrega ala parte superior
template<class T>
void Queue<T>::reheapdown(int root, int bottom)
{
	int minchild, firstchild, child;

	firstchild = root * D + 1; //la posisción del primer hijo de la raíz

	if (firstchild <= bottom) //pregunta si el hijo está en la cola
	{
		minchild = firstchild; //el primer hijo es el menor (temporalmente)

		for (int i = 2; i <= D; ++i)
		{
			child = root * D + i; //posisción del segundo hijo
			if (child <= bottom) //pregunta si el hijo está en la cola
			{
				//Si etse es menor que el quese estabelció como menor
				//entonces son intercambiados
				if (*arr[child] < *arr[minchild])
				{
					minchild = child;
				}
			}
		}

		//Si el hijo menor encontrado es menos que su raiz
		//se intercambian y se llama recursivamente a reheapdown() 
		//para que corrija el orden del arbol hacia abajo
		if (*arr[root] > *arr[minchild])
		{
			swap(arr[root], arr[minchild]);
			reheapdown(minchild, bottom);
		}
	}
}

// se intercambia el valor de dos variables
template<class T>
void Queue<T>::swap(T* &a, T* &b)
{
	T* c;
	c = a;
	a = b;
	b = c;
}

// destructor del arreglo
template<class T>
Queue<T>::~Queue(void)
{
	delete[] arr;
}


// Arbol de Huffman
class Tree
{
private:
	class Node
	{
	public:
		unsigned int freq;
		unsigned char ch;
		Node *left, *right;
		//constructor
		Node(void)
			:freq(0), ch('\0'), left(NULL), right(NULL) {}
	};

	Node *root;

	Tree(const Tree &); //copia el constructor
	const Tree & operator=(const Tree &); //assignment oper. overload
	void chop(Node * N); //destruye el arbol
	void print(ostream &, Node *, int) const; //imprime el arbol
	void print(Node *, int) const; //imprime el arbol

public:
	Tree(void); //constructor
	~Tree(void); //destructor
	friend ostream & operator<<(ostream &, const Tree &);
	//Funciones de utilidad para clase get y set
	unsigned int get_freq(void) const;
	unsigned char get_char(void) const;
	void set_freq(unsigned int);
	void set_char(unsigned char);
	Node* get_left(void) const;
	Node* get_right(void) const;
	void set_left(Node *);
	void set_right(Node *);
	Node* get_root(void) const;
	//compara dos objetos de la clase
	bool operator==(const Tree &) const;
	bool operator!=(const Tree &) const;
	bool operator<(const Tree &) const;
	bool operator>(const Tree &) const;
	bool operator<=(const Tree &) const;
	bool operator>=(const Tree &) const;

	//obtiene la cadena de Huffman para un char
	void huf(Node *, unsigned char, string, string &) const;
	//retorna la cadena de Huffman para un char
	void huf_list(Node *, string) const;
	//obtiene el equivalente en char de una cadena de HUffman, si existe
	bool get_huf_char(string, unsigned char &) const;
	string print_char(Node *) const; //imprime los chars
};

//constructor
Tree::Tree(void)
{
	Node* N = new Node;
	root = N;
}

//función recursiva para destruir el arbol
void Tree::chop(Node *N)
{
	if (N)
	{
		chop(N->left);
		chop(N->right);
		delete N;
	}
}

//destructor de objetos en el arbol
Tree::~Tree(void)
{
	chop(root);
}

unsigned int Tree::get_freq(void) const
{
	return root->freq;
}

unsigned char Tree::get_char(void) const
{
	return root->ch;
}

void Tree::set_freq(unsigned int f)
{
	root->freq = f;
}

void Tree::set_char(unsigned char c)
{
	root->ch = c;
}

Tree::Node* Tree::get_left(void) const
{
	return root->left;
}

Tree::Node* Tree::get_right(void) const
{
	return root->right;
}

void Tree::set_left(Node* N)
{
	root->left = N;
}

void Tree::set_right(Node* N)
{
	root->right = N;
}

Tree::Node* Tree::get_root(void) const
{
	return root;
}

//la salida recursiva del arbol
void Tree::print(ostream & ost, Node * curr, int level) const
{
	if (curr) //si el nodo actual no está lleno, entonces
	{
		print(ost, curr->right, level + 1); //intenta ir al nodo derecho
											//retorna los datos del nodo con respecto a su nivel
		ost << setw(level*width_unit) << print_char(curr) << ":"
			<< curr->freq << endl;
		print(ost, curr->left, level + 1); //intenta ir al nodo izquierdo
	}
}

//Impresión recursiva del arbol
void Tree::print(Node * curr, int level) const
{
	if (curr) //si el nodo actusal no está lleno
	{
		print(curr->right, level + 1); //intenta ir al nodo derecho
									   //retorna los datos del nodo con respecto a su nivel
		cout << setw(level*width_unit) << print_char(curr) << ":"
			<< curr->freq << endl;
		print(curr->left, level + 1); //intenta ir al nodo izquierdo
	}
}

//función de utilidad para retornar el arbol
ostream & operator<<(ostream &ost, const Tree &t)
{
	t.print(ost, t.root, 1);
	return ost;
}

//Comparación de dos árboles de Huffman

bool Tree::operator==(const Tree & T) const
{
	return (root->freq == T.root->freq);
}

bool Tree::operator!=(const Tree & T) const
{
	return (root->freq != T.root->freq);
}

bool Tree::operator<(const Tree & T) const
{
	return (root->freq < T.root->freq);
}

bool Tree::operator>(const Tree & T) const
{
	return (root->freq > T.root->freq);
}

bool Tree::operator<=(const Tree & T) const
{
	return (root->freq <= T.root->freq);
}

bool Tree::operator>=(const Tree & T) const
{
	return (root->freq >= T.root->freq);
}

//Búsqueda de strings de Hufmman (func.  recursiva)
//entradas : a tree node to start the search, a char to find its
//        Huffman string equivalent, current Huffman string according to
//        position on the Huffman tree, and a string (by reference) to
//        copy the resulted full Huffman string end of the search
//salidas: none (except Huffman string by reference)
void Tree::huf(Node* N, unsigned char c, string str, string & s) const
{
	if (N) //if the node is not null
	{
		//compare char of the leaf node and the given char
		if (!N->left && !N->right && N->ch == c)
		{
			s = str; //if the char is found then copy the H. string
		}
		else
		{
			//continue to search if the same char still not found
			huf(N->left, c, str + "0", s);
			huf(N->right, c, str + "1", s);
		}
	}
}

//Huffman char-string lister (func.  recursiva)
//entradas : a tree node to start the search, current Huffman string according to
//        position on the Huffman tree
//output: whole list of char-H. string code list of the H. tree
void Tree::huf_list(Node* N, string str) const
{
	if (N) //if the node is not null
	{
		if (!N->left && !N->right) //if it is a leaf node
			cout << print_char(N) << " " << str << endl;
		else
		{
			//continue to search
			huf_list(N->left, str + "0");
			huf_list(N->right, str + "1");
		}
	}
}

//char finder with given Huffman string
//entradas : a Huffman string to traverse on the H. tree and
//        a u. char by ref. to copy the char found
//salidas: true if a char is found else false
bool Tree::get_huf_char(string s, unsigned char & c) const
{
	Node * curr = root;
	for (unsigned int i = 0; i<s.size(); ++i)
	{
		if (s[i] == '0') //go to left in the H. tree
			curr = curr->left;
		if (s[i] == '1') //go to right in the H. tree
			curr = curr->right;
	}

	bool found = false;

	if (!curr->left && !curr->right) //if it is a leaf node
	{
		found = true;
		c = curr->ch;
	}

	return found;
}

//entradas : a H. tree node
//salidas: the same char as string or if the char is not printable
//        then its octal representation in the ASCII char set
string Tree::print_char(Node * N) const
{
	string s = "";

	if (!N->left && !N->right) //if it is a leaf node
	{
		unsigned char c = N->ch;

		//if the char is not printable then output its octal ASCII code
		if (iscntrl(c) || c == 32) //32:blank char
		{
			//calculate the octal code of the char (3 digits)
			char* cp = new char;
			for (int i = 0; i<3; ++i)
			{
				sprintf(cp, "%i", c % 8);
				c -= c % 8;
				c /= 8;
				s = (*cp) + s;
			}
			s = '/' + s; // adds \ in front of the octal code
		}
		else
			s = c;
	}
	return s;
}

//the given bit will be written to the output file stream
//entradas : unsigned char i(:0 or 1 : bit to write ; 2:EOF) 
void huf_write(unsigned char i, ofstream & outfile)
{
	static int bit_pos = 0; //0 to 7 (left to right) on the byte block
	static unsigned char c = '\0'; //byte block to write

	if (i<2) //if not EOF
	{
		if (i == 1)
			c = c | (i << (7 - bit_pos)); //add a 1 to the byte
		else //i==0
			c = c & static_cast<unsigned char>(255 - (1 << (7 - bit_pos))); //add a 0
		++bit_pos;
		bit_pos %= 8;
		if (bit_pos == 0)
		{
			outfile.put(c);
			c = '\0';
		}
	}
	else
	{
		outfile.put(c);
	}
}

//entradas : a entradas file stream to read bits
//salidas: unsigned char (:0 or 1 as bit read or 2 as EOF) 
unsigned char huf_read(ifstream & infile)
{
	static int bit_pos = 0; //0 to 7 (left to right) on the byte block
	static unsigned char c = infile.get();

	unsigned char i;

	i = (c >> (7 - bit_pos)) % 2; //get the bit from the byte
	++bit_pos;
	bit_pos %= 8;
	if (bit_pos == 0)
		if (!infile.eof())
		{
			c = infile.get();
		}
		else
			i = 2;

	return i;
}

//Huffman Encoder
void encoder(string ifile, string ofile, bool verbose)
{
	ifstream infile(ifile.c_str(), ios::in | ios::binary);
	if (!infile)
	{
		cerr << ifile << " could not be opened!" << endl;
		exit(1);
	}

	if (ifstream(ofile.c_str()))
	{
		cerr << ofile << " already exists!" << endl;
		exit(1);
	}

	//open the output file
	ofstream outfile(ofile.c_str(), ios::out | ios::binary);
	if (!outfile)
	{
		cerr << ofile << " could not be opened!" << endl;
		exit(1);
	}

	//array to hold frequency table for all ASCII characters in the file
	unsigned int f[256];
	for (int i = 0; i<256; ++i)
		f[i] = 0;

	//read the whole file and count the ASCII char table frequencies
	char c;
	unsigned char ch;
	while (infile.get(c))
	{
		ch = c;
		++f[ch];
	}

	infile.clear(); //clear EOF flag
	infile.seekg(0); //reset get() pointer to beginning

	Queue<Tree> q(3); //use a 3-(priority)heap
	Tree* tp;

	for (int i = 0; i<256; ++i)
	{
		//output char freq table to the output file
		//divide 32 bit u. int values into 4 bytes
		outfile.put(static_cast<unsigned char>(f[i] >> 24));
		outfile.put(static_cast<unsigned char>((f[i] >> 16) % 256));
		outfile.put(static_cast<unsigned char>((f[i] >> 8) % 256));
		outfile.put(static_cast<unsigned char>(f[i] % 256));

		if (f[i]>0)
		{
			//send freq-char pairs to the priority heap as Huffman trees
			tp = new Tree;
			(*tp).set_freq(f[i]);
			(*tp).set_char(static_cast<unsigned char>(i));
			q.enq(tp);
		}
	}

	//construct the main Huffman Tree
	Tree* tp2;
	Tree* tp3;

	do
	{
		tp = q.deq();
		if (!q.empty())
		{
			//get the 2 lowest freq. H. trees and combine them into one
			//and put back into the priority heap
			tp2 = q.deq();
			tp3 = new Tree;
			(*tp3).set_freq((*tp).get_freq() + (*tp2).get_freq());
			(*tp3).set_left((*tp).get_root());
			(*tp3).set_right((*tp2).get_root());
			q.enq(tp3);
		}
	} while (!q.empty()); //until all sub-trees combined into one

						  //find H. strings of all chars in the H. tree and put into a string table
	string H_table[256];
	unsigned char uc;
	for (unsigned short us = 0; us<256; ++us)
	{
		H_table[us] = "";
		uc = static_cast<unsigned char>(us);
		(*tp).huf((*tp).get_root(), uc, "", H_table[us]);
	}

	if (verbose)
	{
		cout << *tp << endl; //output the Huffman tree
							 //output the char-H. string list 
		(*tp).huf_list((*tp).get_root(), "");
		cout << "frequency table is " << sizeof(unsigned int) * 256 << " bytes" << endl;
	}

	unsigned int total_chars = (*tp).get_freq();
	cout << "total chars to encode:" << total_chars << endl;

	//output Huffman coded chars into the output file
	unsigned char ch2;
	while (infile.get(c))
	{
		ch = c;
		//send the Huffman string to output file bit by bit
		for (unsigned int i = 0; i<H_table[ch].size(); ++i)
		{
			if (H_table[ch].at(i) == '0')
				ch2 = 0;
			if (H_table[ch].at(i) == '1')
				ch2 = 1;
			huf_write(ch2, outfile);
		}
	}
	ch2 = 2; // send EOF
	huf_write(ch2, outfile);

	infile.close();
	outfile.close();

} //end of Huffman encoder

  //Huffman Decoder
void decoder(string ifile, string ofile, bool verbose)
{
	ifstream infile(ifile.c_str(), ios::in | ios::binary);
	if (!infile)
	{
		cerr << ifile << " could not be opened!" << endl;
		exit(1);
	}

	if (ifstream(ofile.c_str()))
	{
		cerr << ofile << " already exists!" << endl;
		exit(1);
	}

	//open the output file
	ofstream outfile(ofile.c_str(), ios::out | ios::binary);
	if (!outfile)
	{
		cerr << ofile << " could not be opened!" << endl;
		exit(1);
	}

	//read frequency table from the input file
	unsigned int f[256];
	char c;
	unsigned char ch;
	unsigned int j = 1;
	for (int i = 0; i<256; ++i)
	{
		//read 4 bytes and combine them into one 32 bit u. int value
		f[i] = 0;
		for (int k = 3; k >= 0; --k)
		{
			infile.get(c);
			ch = c;
			f[i] += ch * (j << (8 * k));
		}
	}

	//re-construct the Huffman tree
	Queue<Tree> q(3); //use a 3-(priority)heap (again)
	Tree* tp;

	for (int i = 0; i<256; ++i)
	{
		if (f[i]>0)
		{
			//send freq-char pairs to the priority heap as Huffman trees
			tp = new Tree;
			(*tp).set_freq(f[i]);
			(*tp).set_char(static_cast<unsigned char>(i));
			q.enq(tp);
		}
	}

	//construct the main Huffman Tree (as in Encoder func.)
	Tree* tp2;
	Tree* tp3;

	do
	{
		tp = q.deq();
		if (!q.empty())
		{
			//get the 2 lowest freq. H. trees and combine them into one
			//and put back into the priority heap
			tp2 = q.deq();
			tp3 = new Tree;
			(*tp3).set_freq((*tp).get_freq() + (*tp2).get_freq());
			(*tp3).set_left((*tp).get_root());
			(*tp3).set_right((*tp2).get_root());
			q.enq(tp3);
		}
	} while (!q.empty()); //until all sub-trees combined into one

	if (verbose)
	{
		cout << *tp << endl; //output the Huffman tree
							 //output the char-H. string list 
		(*tp).huf_list((*tp).get_root(), "");
		cout << "frequency table is " << sizeof(unsigned int) * 256 << " bytes" << endl;
	}

	//read Huffman strings from the input file
	//find out the chars and write into the output file
	string st;
	unsigned char ch2;
	unsigned int total_chars = (*tp).get_freq();
	cout << "total chars to decode:" << total_chars << endl;
	while (total_chars>0) //continue until no char left to decode 
	{
		st = ""; //current Huffman string
		do
		{
			//read H. strings bit by bit
			ch = huf_read(infile);
			if (ch == 0)
				st = st + '0';
			if (ch == 1)
				st = st + '1';
		} //search the H. tree
		while (!(*tp).get_huf_char(st, ch2)); //continue until a char is found

											  //output the char to the output file
		outfile.put(static_cast<char>(ch2));
		--total_chars;
	}

	infile.close();
	outfile.close();

} //end of Huffman decoder

void usage_msg(const string & pname)
{
	cerr << "Usage: " << pname << " : valid flags are :" << endl
		<< " -i input_file  : required" << endl
		<< " -o output_file : required" << endl
		<< " -e  : turn on encoding mode ( default )" << endl
		<< " -d  : turn on decoding mode" << endl
		<< " -v  : verbose mode" << endl
		<< " -h  : this help screen" << endl;
}

int main(int argc, char * argv[])
{
	string in_name;
	string out_name;
	bool encode = true;
	bool verbose = false;

	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp("-h", argv[i]) || !strcmp("--help", argv[i]))
		{
			usage_msg(argv[0]);
			exit(HELP_ERROR);
		}
		else if (!strcmp("-i", argv[i]))
		{
			cerr << "input file is '" << argv[++i] << "'" << endl;
			in_name = argv[i];
		}
		else if (!strcmp("-o", argv[i]))
		{
			cerr << "output file is '" << argv[++i] << "'" << endl;
			out_name = argv[i];
		}
		else if (!strcmp("-d", argv[i]))
		{
			encode = false;
		}
		else if (!strcmp("-e", argv[i]))
		{
			encode = true;
		}
		else if (!strcmp("-v", argv[i]))
		{
			verbose = true;
		}

	}
	if (!in_name.size() || !out_name.size())
	{
		cerr << "input and output file are required, nothing to do!" << endl;
		usage_msg(argv[0]);
		exit(HELP_ERROR);
	}

	if (encode)
	{
		cerr << "running in encoder mode" << endl;
		encoder(in_name, out_name, verbose);
	}
	else
	{
		cerr << "running in decoder mode" << endl;
		decoder(in_name, out_name, verbose);
	}
	cerr << "done .... " << endl;

	return 0;
}
