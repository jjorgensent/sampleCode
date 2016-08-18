/***********************************************************************
* Program:
*    Assignment 256, Final Assignment
*    Brother Ercanbrack, CS235
* Author:
*    James Jorgensen
* Summary: 
*    This assignment will be able to parse data from a GEDCOM format
*    file and be able to sort the data by name, and then output a text
*    format family tree.
*    
*    Estimated:  20.0 hrs   
*    Actual:     15.0 hrs
*      Please describe briefly what was the most difficult part.
************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

/***********************************************************************
 * Data struct - stores information for each record number
 ***********************************************************************/
struct Data
{
   string recordNum;
   string given;
   string last;
   string birthdate;

   bool operator < (const Data &rhs) const;
   bool operator > (const Data &rhs) const;
   friend ostream& operator << (ostream& out, Data display);
};

/****************************************************************************
 * Node class - stores nodes for linked list and for a family tree
 ****************************************************************************/
class Node
{
   private:
      Data data;
      Node* next;
      Node* father;
      Node* mother;

   public:
      Node();
      Node(Data item);
      Node* getFather(void);
      Node* getMother(void);
      Node* getNext(void);  
      Data getData(void);
      void setData(Data item);
      void setNext(Node* item);
      void setFather(Node* newDad);     
      void setMother(Node* newMom);
      void level(void);

};

/***************************************************************************
 * FTree class - contains subject of the family tree as root
 ***************************************************************************/
class FTree
{
   private:
      Node* root;  // first individual record 
      Node* back; //back of list     
      int size;

   public:
      FTree(void);
      FTree(const FTree &newList);
      ~FTree(void);
      Node* getRoot(void);
      int getSize(void);
      Node* lookup(string findRecord);
      void popFront(void);
      void pushBack(Node* newNode);
      void sort(void);
      void level(void);              

      friend ostream& operator << (ostream& out, FTree list);
      friend bool isSorted(FTree sortMe);
};

//public functions
bool writeFile(char fileName[], FTree sortMe);
bool readFile(char fileName[], FTree &list);

/**********************************************************************
 *
 *
 * BEGIN DATA STRUCT FUNCTION DEFINITIONS
 *
 *
 **********************************************************************/

/**********************************************************************
 *  (<) operator - compares Data objects to determine if lhs is less
 *  than rhs
 **********************************************************************/
bool Data::operator < (const Data &rhs) const
{
   string temp1 = last;
   string temp2 = rhs.last;

   //to upper for first char of last name
   if (temp1[0] <= 'z' && temp1[0] >= 'a')
      temp1[0] += 'A' - 'a';

   if (temp2[0] <= 'z' && temp2[0] >= 'a')
      temp2[0] += 'A' - 'a';

   //compare last name
   if (temp1 < temp2)
      return true;
   else if (temp1 > temp2)
      return false; 

   //compare given
   temp1 = given;
   temp2 = rhs.given;

   if (temp1 < temp2)
      return true;
   else if (temp1 > temp2)
      return false;

   //compare birthday (just year)
   temp1 = birthdate;
   temp2 = rhs.birthdate;

   while (temp1.size() > 4)
      temp1.erase(0, 1);
 
   while (temp2.size() > 4)
      temp2.erase(0, 1);

   if (temp1 < temp2)
      return true;

   //all conditions checked must be false
   return false;
}

/********************************************************************
 * (>) operator - compares Data objects to determine if lhs is less
 * thank rhs
 *******************************************************************/
bool Data::operator > (const Data &rhs) const
{

   string temp1 = last;
   string temp2 = rhs.last;

   //to upper for first char of last name
   if (temp1[0] <= 'z' && temp1[0] >= 'a')
      temp1[0] += 'A' - 'a';

   if (temp2[0] <= 'z' && temp2[0] >= 'a')
      temp2[0] += 'A' - 'a';

   //compare last name
   if (temp1 > temp2)
      return true;
   else if (temp1 < temp2)
      return false;

   //compare given name
   temp1 = given;
   temp2 = rhs.given;

   if (temp1 > temp2)
      return true;
   else if (temp1 < temp2)
      return false;

   //compare birthdate (just year)
   temp1 = birthdate;
   temp2 = rhs.birthdate;

   while (temp1.size() > 4)
      temp1.erase(0, 1);
 
   while (temp2.size() > 4)
      temp2.erase(0, 1);

   if (temp1 > temp2)
      return true;

   //every condition handled must be false
   return false;
}


/**********************************************************************
 * (<<) operator - overloaded extraction operator to simplifiy output
 * of Data objects
 **********************************************************************/
ostream& operator << (ostream& out, Data display)
{

   out << display.given;
   if (display.given != "\0" && display.last != "\0")
      out << ' ';
   out << display.last;
   if (display.birthdate != "\0")
      out << ", b. " << display.birthdate;

   out << endl;    

   return out;

}


/**********************************************************************
 *
 *
 * BEGIN NODE CLASS FUNCTION DEFINITIONS
 *
 *
 **********************************************************************/
/**********************************************************************
* default constructor - assigns value of data to the new nodes data
* defaults pointers to NULL
***********************************************************************/
Node::Node()
{
   data.recordNum = '\0';
   data.given = '\0';
   data.last = '\0';
   data.birthdate = '\0';

   next = NULL;  
   father = NULL;
   mother = NULL;

}

/**********************************************************************
 * Copy constructor - creates a new node with same data values (links
 * excluded to create a copy in memory
 **********************************************************************/
Node::Node(Data item)
{
   this->data.recordNum = item.recordNum;
   this->data.given = item.given;
   this->data.last = item.last;
   this->data.birthdate = item.birthdate;

   next = NULL;
   father = NULL;
   mother = NULL;
}

/***********************************************************************
 * getNext - returns next node in the linked list
 ***********************************************************************/
Node* Node::getNext(void)
{
   return (next);
}

/***********************************************************************
 * getFather - gets father of the current node
 ***********************************************************************/
Node* Node::getFather(void)
{
   return (father);
}

/**********************************************************************
* getMother- returns mother of the current node
***********************************************************************/
Node* Node::getMother(void)
{
   return (mother);
}

/**********************************************************************
* getData - returns Data object in current node
***********************************************************************/
Data Node::getData(void)
{
   return data;
}

/**********************************************************************
* setData - sets data to item value
***********************************************************************/
void Node::setData(Data item)
{
   data = item;
}

/**********************************************************************
* setFather - sets father node of current node to newDad
***********************************************************************/
void Node::setFather(Node* newDad)
{
   father = newDad;
}

/**********************************************************************
* setMom - sets mother node of current node to newMom
***********************************************************************/
void Node::setMother(Node* newMom)
{
   mother = newMom;
}

/**********************************************************************
 * setNext - sets next node in the linked list
 **********************************************************************/
void Node::setNext(Node* newNode)
{
   next = newNode;
}

/**********************************************************************
 * level - outputs family tree by generations
 ***********************************************************************/
void Node::level()
{
   const int MAX = 200;
   Node *queue[MAX];
   Node *temp;
   string displayGen[6] 
      = { "Parents:"               ,           "Grandparents:", 
          "Great Grandparents:"    , "2nd Great Grandparents:", 
          "3rd Great Grandparents:", "th Great Grandparents:"};

   int front = 0;
   int back = 0;
   queue[back++] = this;
   int setBack = back;
   int generation = 1; 

   temp = queue[front];
   front = (front + 1) % MAX;

   cout << "The Ancestors of " << temp->getData().given << ' ' 
                               << temp->getData().last << ":\n";

   queue[back] = temp->father;
   back = (back + 1) % MAX;
   queue[back] = temp->mother;
   back = (back + 1) % MAX;
   setBack = back + 1;
   cout << displayGen[0] << endl;

   //begin traversal
   while (front != back)
   {
      temp = queue[front];
      front = (front + 1) % MAX;
      if (temp != NULL)
      {
         if (front == setBack)
         {
            if (generation > 4)
               cout << generation - 1 << displayGen[5] << endl;
            else 
               cout << displayGen[generation] << endl;
            generation++;
            setBack = back + 1;
         }

         // visit
         cout << "        " << temp->getData();

         queue[back] = temp->father;
         back = (back + 1) % MAX;
         queue[back] = temp->mother;
         back = (back + 1) % MAX;
      }
   }//end traversal
}

/*********************************************************************
 *
 *
 * BEGIN FTREE CLASS FUNCTION DEFINITIONS
 *
 *
 *********************************************************************/
/*********************************************************************
 * default constructor - creates a new family tree object
 *********************************************************************/
FTree::FTree()
{
   root = NULL;
   back = NULL;
   size = 0;
}

/*********************************************************************
 * copy constructor - copies Data values but not addresses to make
 * exact copy in memory
 *********************************************************************/
FTree::FTree(const FTree &newList)
{

   Node* temp;
   Node* temp2;

   size = 0;
   temp = newList.root;
   back = NULL;
   root = new Node(temp->getData());
   size++;

   //makes copy of data into new list
   while (temp->getNext())
   {
      temp = temp->getNext();
      temp2 = new Node(temp->getData());
      pushBack(temp2);
   }
}

/**********************************************************************
* destructor - does nothing
***********************************************************************/
FTree::~FTree()
{

}

/**********************************************************************
 * (<<) operator - overloaded extraction operator for easy output of 
 * family tree list
 **********************************************************************/
ostream& operator << (ostream& out, FTree list)
{

   Node* ptr = list.getRoot();

   for (ptr; ptr; ptr = ptr->getNext())
   {
      Data temp = ptr->getData();
      out << temp;
   }

   return out;
}

/**********************************************************************
 * getRoot - returns subject (root) of tree
 **********************************************************************/
Node* FTree::getRoot(void)
{
   return (root);
}

/**********************************************************************
 * getSize - returns number of individual records within the tree
 **********************************************************************/
int FTree::getSize(void)
{
   return size;
}

/****************************************************************************
 * lookup - searches family tree list by record number
 ****************************************************************************/
Node* FTree::lookup(string findRecord)
{
   if (findRecord == root->getData().recordNum)
      return root;

   Node* temp = root->getNext();

   for (temp; temp; temp = temp->getNext())
   {
      if (temp->getData().recordNum == findRecord)
         break;
   }

   return temp;
}

/****************************************************************************
 * sort - creates a new instance of the family tree, sorts it, and outputs 
 * to a sorted.dat file
 ***************************************************************************/
void FTree::sort()
{
   FTree split1;
   FTree split2;
   FTree sortMe(*this);
 
   //split sortMe into two lists
   while (!isSorted(sortMe))
   {
      while (sortMe.getSize() > 0)
      {
         Node* temp1 = new Node(sortMe.root->getData());
         split1.pushBack(temp1);
         sortMe.popFront();

         while ((sortMe.getSize() > 0) && 
                (sortMe.root->getData() > split1.back->getData()))
         {
            Node* temp = new Node(sortMe.root->getData());
            split1.pushBack(temp);
            sortMe.popFront();
         }

         if (sortMe.getSize() > 0)
         {
            Node* temp = new Node(sortMe.root->getData());
            split2.pushBack(temp);
            sortMe.popFront();
         }  

         while ((sortMe.getSize() > 0) && 
                (sortMe.root->getData() > split2.back->getData()))
         {
            Node* temp = new Node(sortMe.root->getData());
            split2.pushBack(temp);
            sortMe.popFront();
         }
      }//end while split

      //merge lists back into sortMe
      while (split1.getSize() || split2.getSize())
      {
         if (split1.getSize() == 0)
            while (split2.getSize() > 0)
            {
               Node* temp = new Node(split2.root->getData());
               sortMe.pushBack(temp);
               split2.popFront();
            }

         else if (split2.getSize() == 0)
            while (split1.getSize() > 0)
            {
               Node* temp = new Node(split1.root->getData());
               sortMe.pushBack(temp);
               split1.popFront();
            }

         else
         {
            while ((split1.getSize() > 0) && 
                   (split1.root->getData() < split2.root->getData()))
            {
               Node* temp = new Node(split1.root->getData());
               sortMe.pushBack(temp);
               split1.popFront();
            }

            if (split1.getSize() > 0)
               while ((split2.getSize() > 0) && 
                      (split2.root->getData() < split1.root->getData()))
               {
                  Node* temp = new Node(split2.root->getData());
                  sortMe.pushBack(temp);
                  split2.popFront();
               }
         }//end else
      }//end while split size checks for merge
   }//end sort

   char fileName[] = "sorted.dat";
   writeFile(fileName, sortMe);
}

/***********************************************************************
 * pushBack - pushes a new node onto back of family tree list
 **********************************************************************/
void FTree::pushBack(Node* newNode)
{
   if (root == NULL)
   {
      root = newNode;
      back = root;
      size++;
      return;
   }

   Node* ptr = root;
 
   //find end of list
   while (ptr->getNext())
      ptr = ptr->getNext();

   //add newNode to list
   ptr->setNext(newNode);
   back = newNode;
   size++;
}

/************************************************************************
 * level - outputs family tree by generations
 ************************************************************************/
void FTree::level()
{
   root->level();
}

/************************************************************************
 * popFront - removes first node from tree
 ************************************************************************/
void FTree::popFront()
{

   if (root == NULL)
   {
      return;
   }

   //memory leak here, not sure how to create a temp node without 
   //seg faulting during runtime
   if (root->getNext() != NULL)
   {
      root = root->getNext();
   }
   else
      root = NULL;

   size--;

   return;
}

/****************************************************************************
 * isSorted - determines if a tree is sorted by name without altering
 * incoming list 
 ****************************************************************************/
bool isSorted(FTree sortMe)
{

   Node* temp;
   Node* temp2;
   Data tempData;
   Data temp2Data;

   while (sortMe.getSize() > 0)
   {
      temp = sortMe.getRoot();
      sortMe.popFront();
      if (sortMe.getSize() > 0)
      {
         if (temp->getData() > sortMe.getRoot()->getData())
            return false;
      }
   }

   return true;
}

/**************************************************************************
 *
 *
 * BEGIN PUBLIC FUNCTION DEFINITIONS
 *
 *
 **************************************************************************/
/**************************************************************************
 * writeFile - writes family tree out to file
 **************************************************************************/
bool writeFile(char fileName[], FTree sortMe)
{
   ofstream output;

   output.open(fileName);

   if (output.fail())
      return false;

   output << sortMe;

   output.close();

   return true;
}

/****************************************************************************
 * readFile - reads in and parses a gedcom formatted file and builds a family
 * tree based on data input
 ****************************************************************************/
bool readFile(char fileName[], FTree &list)
{

   ifstream input;
   input.open(fileName);

   if (input.fail())
   {
      cout << "ERROR: Invalid filename entered on command line.\n";
      return 1;
   }

   char code;
   string readIn;
   Data temp;

   while (input >> code)
   {
      switch (code)
      {
         case '0':
         {
            input >> readIn;

            //found record number
            if (readIn[0] == '@') 
            {
               //individual record number
               if (readIn[1] == 'I')
               {
                  readIn.erase(0, 1);
                  readIn.erase(readIn.length() - 1, 1);
                  temp.recordNum = readIn;
               }

               //family record number
               else if (readIn[1] == 'F')
               {
                  //build tree here
                  string father;
                  string mother;
                  string child;
                  char temp;

                  //throw away rest of line and next
                  getline(input, readIn);
                  getline(input, readIn);

                  while (input>> temp)
                  {
                     //designates family member
                     if (temp == '1')
                     {
                        input >> readIn;
                        if (readIn == "HUSB")
                        {
                           input >> father;
                           father.erase(0, 1);
                           father.erase(father.length() - 1, 1);
       	                }

                        else if (readIn == "WIFE")
                        {
                           input >> mother;
                           mother.erase(0, 1);
                           mother.erase(mother.length() - 1, 1);
                        }

                        else if (readIn == "CHIL")
                        {
                           input >> child;
                           child.erase(0, 1);
                           child.erase(child.length() - 1, 1);
                           Node* temp = list.lookup(father);
                           list.lookup(child)->setFather(temp);
                           father = "\0";
                           temp = list.lookup(mother);
                           list.lookup(child)->setMother(temp);
                           mother = "\0";
                        }
                     }
                  }//end while temp
               }//end else if 'F'
            }//end if '@'
            break;
         }//end case 0

         case '1':
         {
            input >> readIn;
            if (readIn == "BIRT")
            {
               input >> code;
               input >> readIn;

               if (readIn == "DATE")
               {
                  getline (input, temp.birthdate);
                  temp.birthdate.erase(0, 1);
               }
            }
            break;
         }//end case '1'

         case '2':
         {
            input >> readIn;
            if (readIn == "GIVN")
            {
               getline(input, temp.given);
               temp.given.erase(0, 1);
            }

            else if (readIn == "SURN")
            {
               getline(input, temp.last);
               temp.last.erase(0, 1);
            }
            break;
         }// end case '2'

         case '3':
         {
            //individual record complete, make new node
            input >> readIn;

            if (readIn == "TIME")
            {
               Node* newNode = new Node(temp);
               list.pushBack(newNode);
               temp.recordNum = "\0";
               temp.given = "\0";
               temp.last = "\0";
               temp.birthdate = "\0";
            }
            break;
         }//end case 3
      }//end switch
   }// end reading file

   input.close();

   return 0;
}

/*********************************************************************
 * main - driver program controls flow of information in program
 *********************************************************************/
int main(int argc, char* argv[])
{

   if (argc < 2)
   {
      cout << "ERROR: Filename not found on command line\n";
      return -1;
   }

   FTree list;
   if (readFile(argv[argc - 1], list))
      return -1;

   list.level();
   list.sort();

   return 0;
}
