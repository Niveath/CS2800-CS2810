#include<iostream>
using namespace std;

template<class Object>
class DoublyLinkedList
{
    private:
        struct Node
        {
            Object data;
            Node *next;
            Node *prev;
              Node(const Object & d = Object{ }, Node * p = nullptr, Node * n = nullptr) 
              : data(d), prev(p), next(n) {}
        };
          
        Node *head; //sentinel node at the beginning
        Node *rear; //sentinel node at the end
    
    public:
        DoublyLinkedList()
        {
            head = new Node;
            rear = new Node;
            head->next = rear;
            head->prev = NULL;
            rear->next = NULL;
            rear->prev = head;
        }
  
        class Iterator
        {
            private:
                Node *current;        
            public:
                Iterator(Node *inp) : current(inp) {}
                Object & operator*() {return current->data;}
          
                /*TODO: You can add more methods here */
          
            friend class DoublyLinkedList<Object>;
            
            bool operator!= (Iterator itr)
            {
                if(current!=itr.current) return true;
                return false;
            }

            Iterator operator++ (int) 
            {
                Iterator temp(this->current);
                this->current = this->current->next;
                return temp;
            }

            Iterator operator-- ()
            {
                this->current = this->current->prev;
                return *this;
            }
        };

        Iterator begin(){return Iterator(head->next);}    
        Iterator end(){return Iterator(rear);}
    
        Iterator insert(Iterator itr, Object & o)
        {
            Node* cur = itr.current;
            Node* pre = cur->prev;
            Node* new_node;
            new_node = new Node(o);
            new_node->prev = pre;
            new_node->next = cur;
            cur->prev = new_node;
            pre->next = new_node;
            return Iterator(new_node);
        }
    
        Iterator erase(Iterator itr)
        {
            Node* cur = itr.current;
            Node* pre = cur->prev;
            Node* nex = cur->next;
            pre->next = nex;
            nex->prev = pre;
            free(cur);
            return Iterator(nex);
        }
    
        Iterator atIndex(int p)
        {
            Node* temp = head;
            for(int i=0; i<=p; i++)
            {
                if(temp->next!=NULL) temp = temp->next;
                else return Iterator(rear);
            }
            return Iterator(temp);
        }
    
        int indexOf(Iterator itr)
        {
            Node* cur = itr.current;
            Node* temp = head->next;
            int i=0;
            while(temp!=cur)
            {
                if(temp->next!=NULL)
                {
                    temp = temp->next;
                    i++;
                }
            }
            return i;
        }
    
        void display()
        {            
            for (auto it = begin(); it != end(); it++)
                cout << *it << " ";
            cout << endl;
        }        
};

int main()
{
    int N;
    cin >> N;
    DoublyLinkedList<int> dl;
    int n;
    int index;
    int command;
    for (int i = 0; i < N; i++)
    {
        cin >> command;
        switch (command)
        {
            case 1: /*insert the element at an index*/
                cin >> index;
                cin >> n;
                dl.insert(dl.atIndex(index), n);
                break;
            
            case 2: /*erase the element at an index*/
                cin >> index;
                dl.erase(dl.atIndex(index));
                break;

            case 3: /*print the head element*/
                cout << *(dl.begin()) << endl;
                break;

            case 4: /*print the rear element*/
                cout << *(--dl.end()) << endl;
                break;

            case 5: /*print the element at an index*/
                cin >> index;
                cout << *(dl.atIndex(index)) << endl;
                break;

            case 6: /*display the list*/
                dl.display();
                break;

            case 7: /*print the index of the iterator*/
                cin >> index;
                cout << dl.indexOf(dl.atIndex(index)) << endl;
                break;
        }
    }
}