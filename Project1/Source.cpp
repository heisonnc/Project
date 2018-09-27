#include <iostream>
#include<sstream>
#include <queue>
#include <map>
#include <climits> // for CHAR_BIT
#include <iterator>
#include <algorithm>


const int UniqueSymbols = 1 << 8;
const char* SampleString = "";

typedef std::vector<bool> HuffCode;// aqui se almacena el codigo binario del arbol
typedef std::map<char, HuffCode> HuffCodeMap;  // crea un mapa con llave char que sera el caracter que codifico y guarda el codigo  binario para dicho caracter
 
class INode
{
public:
	const int f;

	virtual ~INode() {}

protected:
	INode(int f) : f(f) {}
};

class InternalNode : public INode  // esto es un nodo para el arbol, heredado de un nodo que solo tiene un atributo int
{
public:
	INode *const left;
	INode *const right;

	InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}
	~InternalNode()
	{
		delete left;
		delete right;
	}
};

class LeafNode : public INode  // este nodo guarda los caracter a codificar
{
public:
	const char c;

	LeafNode(int f, char c) : INode(f), c(c) {}
};

struct NodeCmp  // compara la frecuencia de 2 nodos 
{
	bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};

INode* BuildTree(const int(&frequencies)[UniqueSymbols])
{
	std::priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;  // esto inserta ordenado a la cola tree tomando en cuenta la frecuencia 
																	// del caracter
	for (int i = 0; i < UniqueSymbols; ++i){
		if (frequencies[i] != 0)
			trees.push(new LeafNode(frequencies[i], (char)i));
	}
	while (trees.size() > 1){
		INode* childR = trees.top();
		trees.pop();

		INode* childL = trees.top();
		trees.pop();
							// crea un nodo con los 2 ultimos pesos menores  y lo vuelve a insertar a la pila, esto para crear la estructura del codigo de huffman
		INode* parent = new InternalNode(childR, childL);//donde los que tienen menos peso(se repiten menos) estan al final y a la derecha
		trees.push(parent);
	}
	return trees.top();
}

void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes)  // metodo recursivo que genera el codigo binario de la expresion que se le ingreso
{
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node)){
		outCodes[lf->c] = prefix;
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
	{
		HuffCode leftPrefix = prefix;
		leftPrefix.push_back(false);  // aqui es donde se crea el codigo binario false->0 -> izquierda
		GenerateCodes(in->left, leftPrefix, outCodes);

		HuffCode rightPrefix = prefix;
		rightPrefix.push_back(true);// aqui es donde se crea el codigo binario true->1 -> derecha
		GenerateCodes(in->right, rightPrefix, outCodes);
	}
}

int main()
{
	// Build frequency table
	int frequencies[UniqueSymbols] = { 0 };
	std::string pal="";
	std::cout << "Digite la palabra que desea codificar \n";
	std::cin.ignore();
	std::getline(std::cin,pal);
	const char* ptr = pal.c_str();
	while (*ptr != '\0')
		++frequencies[*ptr++];

	INode* root = BuildTree(frequencies);  // crea el arbol que codifica con el metodo de huffman la expresion 

	HuffCodeMap codes;
	GenerateCodes(root, HuffCode(), codes);  // genera el codigo bnario del arbol
	delete root;

	for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
	{
		std::cout << it->first << " ";
		std::copy(it->second.begin(), it->second.end(), std::ostream_iterator<bool>(std::cout));
		std::cout << std::endl;
	}

	std::cout << "\n";
	system("pause");
	return 0;
}