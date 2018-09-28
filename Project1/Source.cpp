#include <iostream>
#include<sstream>
#include <queue>
#include <map>
#include <climits> // for CHAR_BIT
#include <iterator>
#include <algorithm>

//hola mundo
const int UniqueSymbols = 1 << 8;
const char* SampleString = "";

typedef std::vector<bool> HuffCode;// aqui se almacena el codigo binario del arbol
typedef std::map<char, HuffCode> HuffCodeMap;  // crea un mapa con llave char que sera el caracter que codifico y guarda el codigo  binario para dicho caracter
 
//clase padre
class INode{
public:
	const int frequency;
	virtual ~INode() {}
protected:
	INode(int f) : frequency(f) {}
};

// esto es un nodo para el arbol, heredado de un nodo que solo tiene un atributo int 
//Esta clase corresponde al teorema 6.2 el punto 1...
class InternalNode : public INode{
	//internalNode va a tener una frequencia que va a ser igual a la suma de INode left + INode right
public:
	INode *const left;
	INode *const right;
	//En este constructor se suma la frequencia de dos nodos
	InternalNode(INode* c0, INode* c1) : INode(c0->frequency + c1->frequency), left(c0), right(c1) {}
	~InternalNode(){
		delete left;
		delete right;
	}
};

// este nodo guarda los caracter a codificar
class LeafNode : public INode{
public:
	const char c;
	LeafNode(int f, char c) : INode(f), c(c) {}
};

// compara la frecuencia de 2 nodos usando sobrecarga de operador()
struct NodeCmp{
	bool operator()(const INode* lhs, const INode* rhs) const { return lhs->frequency > rhs->frequency; }
};

INode* BuildTree(const int(&frequencies)[UniqueSymbols]){
	//Ordena la cola en 
	// esto inserta ordenado a la cola trees tomando en cuenta la frecuencia del caracter																
	std::priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;  
	for (int i = 0; i < UniqueSymbols; ++i){
		if (frequencies[i] != 0)
			trees.push(new LeafNode(frequencies[i], (char)i));
	}
	while (trees.size() > 1){
		INode* hijoR = trees.top();
		trees.pop();

		INode* hijoI = trees.top();
		trees.pop();
		// crea un nodo con los 2 ultimos pesos menores  y lo vuelve a insertar a la cola,
		//esto para crear la estructura del codigo de huffman
		INode* padre = new InternalNode(hijoR, hijoI);//donde los que tienen menos frecuencia estan al final y a la derecha
		trees.push(padre);
	}
	return trees.top();
}
// metodo recursivo que genera el codigo binario de la expresion que se le ingreso
void generadorCodigo(const INode* nodo, const HuffCode& prefix, HuffCodeMap& outCodes){
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(nodo)){ //Primero crea las hojas
		//ingresa al mapa el tag en este caso el caracter y que almacene el vector
		outCodes[lf->c] = prefix;
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(nodo)){ //Luego con recursividad a los nodos del medio les asigna 0 y 1 hasta llegar a encontrar un caracter
		HuffCode leftPrefix = prefix;
		leftPrefix.push_back(false);  // aqui es donde se crea el codigo binario false->0 -> izquierda
		generadorCodigo(in->left, leftPrefix, outCodes);

		HuffCode rightPrefix = prefix;
		rightPrefix.push_back(true);// aqui es donde se crea el codigo binario true->1 -> derecha
		generadorCodigo(in->right, rightPrefix, outCodes);
	}
}

int main(){
	// Build frequency table
	int frequencies[UniqueSymbols] = { 0 };
	std::string pal="";
	std::cout << "Digite la palabra que desea codificar \n";
	std::cin.clear();
	std::getline(std::cin,pal);
	const char* ptr = pal.c_str();
	while (*ptr != '\0')
		++frequencies[*ptr++];

	INode* root = BuildTree(frequencies);  // crea el arbol que codifica con el metodo de huffman la expresion 

	HuffCodeMap codes;
	// genera el codigo bnario del arbol y lo almacena en el vector llamado HuffCode
	generadorCodigo(root, HuffCode(), codes);  
	delete root;

	for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it){
		std::cout << it->first << " ";
		std::copy(it->second.begin(), it->second.end(), std::ostream_iterator<bool>(std::cout));
		std::cout << std::endl;
	}

	std::cout << "\n";
	system("pause");
	return 0;
}
//aaaaabbbbbbbbbbcccccccddddddddeeeeeeeeeeeeeeefffffffffffffffffffffffffffffffffff