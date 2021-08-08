
#include <iostream>
#include <string>
using namespace std;



enum Actions {
    ACTUAL,
    NEXT,
    PREV,
    ADD_BEG,
    ADD_END,
    ADD_ACT,
    DEL_BEG,
    DEL_END,
    DEL_VAL,
    DEL_ACT,
    PRINT_FORWARD,
    PRINT_BACKWARD,
    END
};

Actions stringToEnum(const string& option) {
    if (option == "ADD_BEG") return ADD_BEG;
    else if (option == "ADD_END") return ADD_END;
    else if (option == "ADD_ACT") return ADD_ACT;
    else if (option == "ACTUAL") return ACTUAL;
    else if (option == "NEXT")   return NEXT;
    else if (option == "PREV") return PREV;
    else if (option == "DEL_ACT") return DEL_ACT;
    else if (option == "DEL_BEG") return DEL_BEG;
    else if (option == "DEL_END") return DEL_END;
    else if (option == "DEL_VAL") return DEL_VAL;
    else if (option == "PRINT_FORWARD") return PRINT_FORWARD;
    else if (option == "PRINT_BACKWARD") return PRINT_BACKWARD;
    else  return END;
}

class Node {
public:
    int data;
    Node* npx;
};


Node* XOR(Node* prev, Node* next) {
    return (Node*)((uintptr_t)prev ^ (uintptr_t)next);
}
int findSize(Node* head)
{
    Node* curr = head;
    Node* next;
    Node* prev = NULL;

    int res = 0;
    while (curr != NULL && res <= 2)
    {
        next = XOR(prev, curr->npx);
        res++;
        prev = curr;
        curr = next;
    }
    return res;
}


Node* findTail(Node* head)
{
    Node* curr = head;
    Node* prev = NULL;
    Node* next;
    Node* tail = NULL;


    while (curr != NULL)
    {
        next = XOR(prev, curr->npx);
        prev = curr;
        curr = next;
    }
    tail = prev;
    return tail;
}

void setActual(Node* head, Node** actual) {
    if (findSize(head) == 1)
        *actual = head;
}





Node* nextAct(Node* actual, Node* head) {
    Node* curr = head;
    Node* prev = NULL;
    Node* next = head;
    Node* tmp = NULL;



    if (curr == NULL) {
        tmp = NULL;
    }
    else if (curr != nullptr) {
        while (curr != actual) {

            next = XOR(prev, curr->npx);
            prev = curr;
            curr = next;
        }
        if (XOR(prev, curr->npx) != NULL)
            tmp = XOR(prev, curr->npx);
        else
            tmp = head;
    }

    return tmp;
}

Node* prevAct(Node* actual, Node* head) {
    Node* curr = head;
    Node* prev = NULL;
    Node* next;
    Node* tmp = NULL;

    if (curr == NULL) {
        tmp = NULL;
    }
    else {
        while (curr != actual) {

            next = XOR(prev, curr->npx);
            prev = curr;
            curr = next;
        }
        if (prev != NULL)
            tmp = prev;
        else
            tmp = findTail(head);
    }
    return tmp;
}



void addOnBeg(Node** head, int data) {
    Node* newBeg = new Node;

    newBeg->data = data;
    newBeg->npx = *head;
    if (*head != NULL) {
        (*head)->npx = XOR(newBeg, (*head)->npx);
    }
    *head = newBeg;


}


void addOnEnd(Node** head, int data) {
    Node* curr = *head;
    Node* prev = NULL;
    Node* next = curr;

    if (curr == NULL) {
        addOnBeg(head, data);
    }
    else {
        while (XOR(curr->npx, prev) != NULL)
        {
            next = XOR(prev, curr->npx);
            prev = curr;
            curr = next;
        }
        Node* tail = new Node;
        tail->data = data;
        curr->npx = XOR(prev, tail);
        tail->npx = XOR(curr, NULL);
    }


}



void addAct(Node** head, Node** actual, int data) {
    Node* curr = *head;
    Node* prev = NULL;
    Node* next = curr;
    Node* temp = *actual;
    if (curr == NULL || temp == *head)
        addOnBeg(head, data);
    else {
        while (curr != temp)
        {
            next = XOR(prev, curr->npx);
            prev = curr;
            curr = next;
        }
        next = XOR(prev, curr->npx);
        Node* newNode = new Node;
        newNode->data = data;
        curr->npx = XOR(newNode, next);
        prev->npx = XOR(XOR(curr, prev->npx), newNode);
        newNode->npx = XOR(prev, curr);

    }
}


void printList(Node* head) {
    Node* curr = head;
    Node* prev = NULL;
    Node* next;
    if (curr == NULL) {
        cout << "NULL";
    }

    while (curr != NULL)
    {
        cout << curr->data << " ";

        next = XOR(prev, curr->npx);

        prev = curr;
        curr = next;
    }
}



void printBack(Node* head) {
    Node* tail = findTail(head);
    printList(tail);
}



Node* delBeg(Node** actual, Node** head) {
    Node* curr = *head;
    if (*head == NULL) {
    }
    else {


        Node* temp = *head;

        *head = XOR(NULL, temp->npx);
        if (*head != NULL) {
            (*head)->npx = XOR(NULL, XOR(temp, (*head)->npx));
        }
        if (XOR(NULL, curr->npx) == NULL)
            *actual = NULL;

        delete temp;
    }
    return *head;
}




Node* delEnd(Node** actual, Node** head)
{
    if (*head == NULL) {
    }
    else {

        Node* curr = *head;
        Node* prev = NULL;
        Node* next;


        while (XOR(curr->npx, prev) != NULL) {
            next = XOR(prev, curr->npx);
            prev = curr;
            curr = next;
        }

        if (prev != NULL)
            prev->npx = XOR(XOR(prev->npx, curr), NULL);


        else {
            *head = NULL;
            *actual = NULL;
        }
        delete curr;
    }

    return *head;
}


void delAct(Node** actual, Node** head) {
    Node* curr = *head;
    Node* prev = NULL;
    Node* next;
    Node* temp = *actual;


    if (curr == NULL) {

    }
    else {
        while (curr != temp) {
            next = XOR(prev, curr->npx);
            prev = curr;
            curr = next;
        }
        next = XOR(prev, curr->npx);
        if (prev != NULL && next != NULL) {
            prev->npx = XOR(XOR(curr, prev->npx), next);
            next->npx = XOR(prev, XOR(curr, next->npx));
            delete temp;
        }
        else if (*actual == *head) {
            delBeg(actual, head);

        }
        else if (*actual == findTail(*head)) {
            delEnd(actual, head);
        }


        if (prev != NULL) {
            *actual = prev;
        }
        else {
            *actual = findTail(*head);
        }
    }
}



void delVal(Node** head, int data) {
    Node* curr = *head;
    Node* prev = NULL;
    Node* next = XOR(prev, curr->npx);

    while (curr != NULL)
    {
        next = XOR(prev, curr->npx);
        if (curr->data == data) {
            delAct(&curr, head);
            curr = prev;


        }
        prev = curr;
        curr = next;

    }

}



void printActual(Node* actual) {
    if (actual != NULL)
        cout << actual->data << endl;
    else
        cout << "NULL" << endl;
}

int main()
{
    string option;
    Actions action;
    int quit = 0;
    int tmp = 0;

    Node* actual;
    Node* xorLinkedList = NULL;
    actual = xorLinkedList;

    while (cin >> option) {
        action = stringToEnum(option);

        switch (action) {
        case ADD_BEG:
            cin >> tmp;
            addOnBeg(&xorLinkedList, tmp);
            setActual(xorLinkedList, &actual);

            break;
        case ADD_END:
            cin >> tmp;
            addOnEnd(&xorLinkedList, tmp);
            setActual(xorLinkedList, &actual);
            break;
        case ADD_ACT:
            cin >> tmp;
            addAct(&xorLinkedList, &actual, tmp);
            setActual(xorLinkedList, &actual);
            break;
        case ACTUAL:
            printActual(actual);
            break;
        case NEXT:
            if (findSize(xorLinkedList))
                actual = nextAct(actual, xorLinkedList);
            else
                actual = NULL;
            printActual(actual);
            break;
        case PREV:
            if (findSize(xorLinkedList))
                actual = prevAct(actual, xorLinkedList);
            else
                actual = NULL;
            printActual(actual);
            break;
        case DEL_ACT:
            delAct(&actual, &xorLinkedList);
            break;
        case DEL_BEG:
            delBeg(&actual, &xorLinkedList);
            break;
        case DEL_END:
            delEnd(&actual, &xorLinkedList);
            break;
        case DEL_VAL:
            cin >> tmp;
            delVal(&xorLinkedList, tmp);
            break;
        case PRINT_FORWARD:
            printList(xorLinkedList);
            cout << endl;
            break;
        case PRINT_BACKWARD:
            printBack(xorLinkedList);
            cout << endl;
            break;
        case END:
            quit++;
            break;

        }
    }

    return 0;
}
