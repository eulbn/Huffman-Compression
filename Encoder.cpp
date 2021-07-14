#include<bits/stdc++.h>
using namespace std;
class node{

	public:
		char Character;
		int nodeValue;
		node *next, *left, *right;
		node(char c = 0, int nV = 0, node *n = NULL, node *l = NULL, node* r = NULL)
		{
			Character = c;
			nodeValue = nV;
			next = n;
			left = l;
			right = r;
		}
};

class HuffmanTreeGenerator{
	private:
		node *head, *tail, *root;
		
	public:
		HuffmanTreeGenerator()//default constructor
		{
			head = tail = root = NULL;
		}
		void add_to_tail(char Character  = 0, int nodeValue = 0)
		{
			if(head == NULL)
				head = tail = new node(Character,nodeValue);
			else
			{
				tail->next = new node(Character,nodeValue);
				tail = tail->next;
			}
		}
		void Display()
		{
			node *tem = head;
			while(tem)
			{
				cout<<tem->Character<<" "<<tem->nodeValue;
				tem = tem->next;
			}
			cout<<endl;
		}
		void count_to_linkedList(string Characters, int frequencies[])
		{
			for(int i = 0 ; i<Characters.length() ; i++)
			{
				this->add_to_tail(Characters[i],frequencies[Characters[i]]);
			}
		}
		void Built_Huffman_Tree()
		{
			while(head->next)//goign forward till next of head is not NULL
			{
				if(head->next == NULL)
					return;
				int NewNodeValue = (head->nodeValue)+(head->next->nodeValue);
				node *tem = head;
				while(tem->next)//going forward till next of tem is not NULL
				{
					if(tem->next->nodeValue <= NewNodeValue)
						tem = tem->next;//going forward till we appropriate place to insert New node value
					else
						break;//if we have found appropriate place
				}
				if(tem->next == NULL)
				{
					this->add_to_tail(0,NewNodeValue);//inserting new node at the end
					tem = tem->next;
				}
				else
				{
					tem->next = new node(0,NewNodeValue,tem->next);//inserting at appropriate pace
					tem = tem->next;
				}
					
				tem->left = head;//adding the head node to the left of newNodeValue inserted node
				head = head->next;
				tem->left->next = NULL;
				
				tem->right = head;//adding the head node to the right of newNodeValue inserted node
				head = head->next;
				tem->right->next = NULL;	
			}
			root = head;
		}
		void Codes(string *Coodes)
		{
			string codde;
			codes(root,codde, Coodes);
		}
		void codeSeach(string temIn,string *temOut,int edgeCount)
		{
			node *tem = root;
			string temp;
			for(int i = 0 ; i<= temIn.length();i++)
			{
				if(i == (temIn.length()-8) && edgeCount)
						i += (8 - edgeCount);
			
				if(tem->left == NULL && tem->right == NULL)
				{
					temp += tem->Character;
					tem = root;
					
						i--;
				}
				else if(temIn[i] == '0')
					tem = tem->left;
				else if(temIn[i] == '1')
					tem = tem->right;
				
			}
			*temOut = temp;

		}
		private:
			void codes(node *tem,string codde,string *Coodes)
			{
				if(tem->left == NULL && tem->right == NULL)
				{
					Coodes[tem->Character] = codde;//saving codes of character at proper indes e.g A will be saved at 65
					return;
				}
				
				codes(tem->left,codde+'0',Coodes);
				
				codes(tem->right,codde+'1',Coodes);
			}
			
};
class Decoder{
	private:
	int frequencies[256];
		string sorted;
		int edgeCount;
		long long totalChar;
	public:	
		Decoder()
		{
			memset(frequencies,0,sizeof(int)*256);
		}
		SortedKeyInput()
		{
			ifstream inSort("SortedKey.txt");
			if(inSort)
			{
				int size;
				char tem;
				
				while(inSort.get(tem))
				{
					sorted +=tem;
				}

				inSort.close();
				ifstream InFrequencies("Frequencies.txt");
				InFrequencies>>edgeCount;
				for(int i= 0 ; i<sorted.length(); i++)
				{
					InFrequencies>>frequencies[sorted[i]];
				}
				InFrequencies.close();
				
				InputFromCompressed();

			}
			
		}
		void InputFromCompressed()
		{
			HuffmanTreeGenerator obj;
			
			obj.count_to_linkedList(sorted,frequencies);
			obj.Built_Huffman_Tree();
			ifstream inCompress("compressedFile.bin",ios::binary);
			ofstream outCompress("UncompressedFile.txt",ios::binary);
			if(inCompress && outCompress )
			{
				string temIn;
				string temOut;
				char temp;
				int j = 0;
				while(inCompress.get(temp))
				{
					temIn += temp;
				}
					string tem;
					for(int i = 0 ; i<temIn.length();i++)
						tem += bitset<8>(temIn[i]).to_string();
						
				
					obj.codeSeach(tem,&temOut,edgeCount);
		
					outCompress<<temOut;
		
				
				inCompress.close();
				outCompress.close();	
			}
		}
	
};

class Encode{
	private:
		int frequencies[256];
		string sorted;
		string Coodes[256];
		string fileName;
	public:
		Encode(){ memset(frequencies,0,sizeof(int)*256);}
		void InputFromFile()
		{
			ifstream check("UncompressedFile.txt",ios::binary);
			if(check &&check.peek() == std::ifstream::traits_type::eof())//checking if the file is empty
			{
				cout<<"Your file is empty.";
				return;
			}
			check.close();
			ifstream In("UncompressedFile.txt",ios::binary);
			
			if(In)
			{
				
				string tem;
				char temp;
				while(In.get(temp))
				{
					frequencies[temp]++;//increasing the frequencies according to their index e.g a = 97
				}
				for(int i = 0 ; i<256 ; i++)
				{
					if(frequencies[i] != 0)
						sorted += i;//adding the characters to remember their indexes
				}
				for(int i = 0 ; i<sorted.length();i++)//might use a different sort
				{
					for(int j = i; j<sorted.length(); j++)
						if(frequencies[sorted[j]] < frequencies[sorted[i]])
						{
							swap(sorted[j],sorted[i]);
						}
				}
				In.close();
				
				GeneratingTree();
				
				WriteToFile();
				
			}
			else
				cout<<"Sorry this file does not exist";
		}
		void GeneratingTree()
		{
			HuffmanTreeGenerator obj;
			obj.count_to_linkedList(sorted,frequencies);
			obj.Built_Huffman_Tree();
			obj.Codes(Coodes);
		}
		void WriteToFile()
		{
			int edgeCount;
			ifstream In("UncompressedFile.txt",ios::binary);
			ofstream Out("compressedFile.bin",ios::binary);
			if(In && Out)
			{
				char temp;
				string tem;
				while(In.get(temp))
				{	
					tem +=Coodes[temp];
				}
					edgeCount = tem.length()%8;
					stringstream sstream(tem);
				    string output;

				    while(sstream.good())
				    {
				        bitset<8> bits;
				        sstream >> bits;
				        char c = char(bits.to_ulong());
				        output += c;
				    }
				   
				    Out<<output;

				
				In.close();
				Out.close();
				//remove("UncompressedFile.txt"); for later use
			}
			
			ofstream SortedKey("SortedKey.txt");
			
			if(SortedKey)
			{

				
			
				SortedKey<<sorted;
				SortedKey.close();
				ofstream Frequencies("Frequencies.txt");
				Frequencies<<edgeCount<<'\n';
				for(int i = 0 ; i<sorted.length(); i++)
				{
					Frequencies<<frequencies[sorted[i]]<<'\n';
				}
				Frequencies.close();

				
			}
			
				
			
		}
		
};

main()
{

	char tem;
	cout<<"Do you want to compress or decompress? c/d"<<endl;
	cin>>tem;
	
	if(tem == 'c' || tem == 'C')
	{
		Encode obj1;
		obj1.InputFromFile();
	}
	else if(tem == 'd' || tem == 'D')
	{
		Decoder obj;
		obj.SortedKeyInput();
	}


}
