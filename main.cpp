#include <iostream>

using namespace std;

template<class T> class Node{
public:
    T val;
    Node * next;
    Node (T v){val=v;next=NULL;};
    Node(){next=NULL;};
    ~Node(){delete next;}
};

template<class T> class LinkedList{
public:
    Node<T> *head;
    Node<T> *tail;
    int len;
    LinkedList(){head=NULL;tail=NULL;len=0;}
    void Add(T x)
    {
        if (len == 0) {
            head = new Node<T>(x);
            tail = head;
        }
        else {
            tail->next = new Node<T>(x);
            tail = tail->next;
        }
        len++;
    }
    void Insert(T x, int idx)
    {
        if (idx>=len-1)
            Add(x);
        else
        {
            Node<T> *tmp = head;
            for (int i = 0; i < idx; i++)
                tmp = tmp->next;
            Node<T> *tmp2 = tmp->next;
            tmp->next = new Node<T>(x);
            tmp->next->next = tmp2;
            len++;
        }
    }
    T Get(int idx)
    {
        if (idx>len)
            return tail->val;
        else
        {
            Node<T> *tmp = head;
            for (int i = 0; i < idx; i++)
                tmp = tmp->next;
            return tmp->val;
        }
    }
    int IndexOf(T a)
    {
        Node<T> *tmp=head;
        for (int i=0;i<len;i++)
        {
            if (tmp->val==a)
                return i;
            tmp=tmp->next;
        }
        return -1;
    }
    void Remove(int idx)
    {
        if (idx>len-1)
            return;
        Node<T> *tmp=head;
        for (int i=0;i<idx-1;i++)
            tmp=tmp->next;
        Node<T> *tmp2=tmp->next;
        tmp->next=tmp->next->next;
        tmp2->next=NULL;
        delete tmp2;

        len--;
        if (idx==len)
            tail=tmp;
    }
    int Size(){
        return len;
    }
    ~LinkedList(){delete head;}
    class Iterator{
    public:
        Node<T> *ele;
        Iterator(Node<T> * x){ele =x;}
        T operator *(){return ele->val;}
        bool HasNext(){return ele->next!=NULL;}
        T Next(){return ele->next->val;}
    };

};

template <class T> class DNode{
public:
    T val;
    DNode * pre;
    DNode *next;
    DNode (T v) {val=v;next=NULL;pre=NULL;}
    DNode () {pre=NULL;next=NULL;}
    ~DNode(){delete pre;delete next;}
};

template <class T> class DoubleLinkedList {
public:
    DNode<T> *head;
    DNode<T> *tail;
    int len;
    DoubleLinkedList(){head=NULL;tail=NULL;len=0;}
    void Add(T x)
    {
        if (len == 0) {
            head = new DNode<T>(x);
            tail = head;
        }
        else {
            tail->next = new DNode<T>(x);
            tail->next->pre=tail;
            tail = tail->next;
        }
        len++;
    }
    void Insert(T x, int idx)
    {
        if (idx>=len-1)
            Add(x);
        else
        {
            DNode<T> *tmp;
            if (len<2*idx)
            {
                tmp= head;
                for (int i = 0; i < idx; i++)
                    tmp = tmp->next;
            }
            else
            {
                tmp=tail;
                for (int i=len-1; i > idx; i-- )
                    tmp = tmp->pre;
            }
            DNode<T> *tmp2 = tmp->next;
            tmp->next = new DNode<T>(x);
            tmp->next->next = tmp2;
            tmp->next->next->pre=tmp->next;
            tmp->next->pre=tmp;
            len++;
        }
    }
    T Get(int idx)
    {
        if (idx>len)
            return tail->val;
        else
        {
            DNode<T> *tmp;
            if (len<2*idx)
            {
                tmp= head;
                for (int i = 0; i < idx; i++)
                    tmp = tmp->next;
            }
            else
            {
                tmp=tail;
                for (int i=len-1; i > idx; i-- )
                    tmp = tmp->pre;
            }
            return tmp->val;
        }
    }
    int IndexOf(T a)
    {
        DNode<T> *tmp=head;
        for (int i=0;i<len;i++)
        {
            if (tmp->val==a)
                return i;
            tmp=tmp->next;
        }
        return -1;
    }

    void Remove(int idx)
    {
        if (idx>len-1)
            return;
        DNode<T> *tmp;
        if (len<2*idx)
        {
            tmp= head;
            for (int i = 0; i < idx-1; i++)
                tmp = tmp->next;
        }
        else
        {
            tmp=tail;
            for (int i=len-1; i > idx-1; i-- )
                tmp = tmp->pre;
        }
        DNode<T> *tmp2=tmp->next;
        tmp->next=tmp->next->next;
        tmp->next->pre=tmp;
        tmp2->next=NULL;
        tmp2->pre=NULL;
        delete tmp2;
        len--;
        if (idx==len)
            tail=tmp;
    }

    int Size(){
        return len;
    }
    ~DoubleLinkedList(){
        DNode<T> *tmp=head;

        while (tmp->next!=NULL){
            tmp=tmp->next;
            DNode<T> *tmp2=tmp->pre;
            tmp2->pre=NULL;
            tmp2->next=NULL;
            delete tmp2;
        }
        tmp->next=NULL;
        tmp->pre=NULL;
        delete tmp;
    }
    class Iterator{
    public:
        DNode<T> *ele;
        Iterator(DNode<T> * x){ele =x;}
        T operator *(){return ele->val;}
        bool HasNext(){return ele->next!=NULL;}
        T Next(){return ele->next->val;}
    };

};

template <class K> class Hasher {
public:
    virtual int Hash(K) =0;

};

template <class K> class EqualityPredicate{
public:
    virtual bool Collision(K,K)=0;
};

template <class K, class V> class HashUnit{
public:
    K key;
    V val;
    HashUnit(K k,V v){key =k; val =v;}
    HashUnit(){};
};

//key value only support char,int,double
template <class K, class V> class HashTable: public Hasher<K>, public EqualityPredicate<K>{
public:
    HashUnit<K,V> **table;
    static int size;
    HashTable(){
        table=new HashUnit<K,V>*[size];
        for (int i=0;i<size;i++)
            table[i]=NULL;
    }
    V Get(K k)
    {
        int h=Hash(k);
        while (table[h]!=NULL && table[h]->key!=k)
            h=(h+1)%size;
        if (table[h]==NULL)
            cout<<"not exist";
        else
            return table[h]->val;
    }

    void Put(K k,V v)
    {
        int h=Hash(k);
        while (table[h]!=NULL && table[h]->key!=k)
            h=(h+1)%size;
        if (table[h]!=NULL)
            delete table[h];
        table[h]=new HashUnit<K,V>(k,v);
    }
    ~HashTable(){
        for (int i=0;i<size;i++)
            if (table[i]!=NULL)
                delete table[i];
        delete table;
    }
    int Hash(K k)
    {
        return int(k)%size;
    }

    bool Collision(K k1,K k2){return Hash(k1)==Hash(k2);}


};

template<class K,class T> int HashTable<K,T>::size=1000;

int main() {


    cout<<"LinkedList test:"<<endl;
    LinkedList<int> b;
    b.Add(7);
    b.Insert(0,0);
    b.Insert(6,1);
    b.Add(8);
    b.Add(19);
    b.Insert(100,3);
    cout<<"old array is:  ";
    for (int i=0;i<b.len;i++)
        cout<<b.Get(i)<<"  ";
    b.Remove(3);
    cout<<endl;
    cout<<"new array is:  ";
    for (int i=0;i<b.len;i++)
        cout<<b.Get(i)<<"  ";
    cout<<endl;
    cout<<"the head is:  ";
    LinkedList<int>::Iterator it=b.head;
    cout<<*it<<endl;
    cout<<"the index of 100 is  "<<b.IndexOf(100)<<endl;

    cout<<"-----------------------------------------"<<endl;

    cout<<"DoubleLinkedList test:"<<endl;
    DoubleLinkedList<int> c;
    c.Add(7);
    c.Insert(0,0);
    c.Insert(6,0);
    c.Add(8);
    c.Add(19);
    c.Insert(100,3);
    cout<<"old array is:  ";
    for (int i=0;i<c.len;i++)
        cout<<c.Get(i)<<"  ";
    c.Remove(3);
    cout<<endl;
    cout<<"new array is:  ";
    for (int i=0;i<c.len;i++)
        cout<<c.Get(i)<<"  ";
    cout<<endl;
    cout<<"the head is:  ";
    DoubleLinkedList<int>::Iterator it_=c.head;
    cout<<*it_<<endl;
    cout<<"the index of 100 is  "<<c.IndexOf(100)<<endl;

    cout<<"-----------------------------------------"<<endl;

    cout<<"HashTable test:"<<endl;
    HashTable<char,string> a;
    a.Put('s',"bulabula");
    a.Put('4',"rsx");
    cout<<a.Get('s')<<endl;


}

