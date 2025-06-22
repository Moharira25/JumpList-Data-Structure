#include "JumpList.h"
#include <stdexcept>

Node::Node(const string& s, Node* next, Node* jump, int gap) {
	data_ = s;
	next_ = next;
	jump_ = jump;
	gap_ = gap;

}

Node::~Node() {
}


JumpList::JumpList() {
	head_ = nullptr;

}

// DO NOT CHANGE
// You do not need to understand the code here to complete the assignment
JumpList::JumpList(int size, const string* arr) {

	const string s[] = {"a", "b", "blah", "c", "cat", "d", "etc", "ooo...", "x", "zzz"};
	const string* sp = (arr == nullptr) ? s : arr;

	Node** npp = new Node*[size];
	npp[size-1] = new Node(sp[size-1], nullptr, nullptr, 0);
	for(int i = size-2; i >= 0; i--)
		npp[i] = new Node(sp[i], npp[i+1], nullptr, 0);

	if (arr == nullptr) {

		if (size==1) npp[0]->gap_ = 1;
		else if (size==3) npp[0]->gap_ = 3;
 		else if (size==10) {
			npp[0]->jump_ = npp[5];
			npp[5]->jump_ = npp[8];
			npp[0]->gap_ = 5;
			npp[5]->gap_ = 3;
			npp[8]->gap_ = 2;
		}
		else throw std::invalid_argument("size must be 1, 3 or 10");

	}
	else {

		// jump node every MAX_GAP_SIZE
		int i;
		for(i=0; i < size-MAX_GAP_SIZE; i += MAX_GAP_SIZE) {
			npp[i]->jump_ = npp[i+MAX_GAP_SIZE];
			npp[i]->gap_ = MAX_GAP_SIZE;
		}
		npp[i]->gap_ = size % MAX_GAP_SIZE;
		if (npp[i]->gap_ == 0) npp[i]->gap_ = MAX_GAP_SIZE; // 0 need to become 5!

	}

	head_ = npp[0];
	delete [] npp; // note that the local array npp gets destroyed here but the nodes stay alive!

}

JumpList::~JumpList() {
	if (head_ == nullptr) return; //empty list

	Node* current = head_;
	while (current != nullptr){
	    Node* nodeToBeDeleted = current;
        current = current->next_;
        delete nodeToBeDeleted;   
	}
}


int JumpList::size() const {
	int listSize = 0;
	Node* current = head_;
	while (current != nullptr){
        listSize += 1;
        current = current->next_;
	}

	return listSize;
}

// DONE FOR YOU, DO NOT CHANGE
bool JumpList::find(const string& s) const {

	if (head_ == nullptr) return false;

	// moving along the fast lane
	Node* tmp = head_;
	while(tmp->jump_ != nullptr && tmp->jump_->data_ < s)
		tmp = tmp->jump_;

	// tmp now points to the jump node at the start of the segment where s could be

	// now slow lane
	while(tmp != nullptr) {
		if (tmp->data_ == s) return true; // match
		else if (tmp->data_ > s) return false; // went past without finding s
		else tmp = tmp->next_;
	}
	return false; // end of list
}

string JumpList::get(int i) const {
	if (head_ == nullptr) return ""; //empty list
	if (i < 0 || i >= size()){
        return ""; //invalid index
	}
	// moving along the fast lane
	Node* tmp = head_;
	int index = 0;
	while(tmp->jump_ != nullptr && index + tmp->gap_ < i){
        index += tmp->gap_;
        tmp = tmp->jump_;
	}
	// tmp now points to the jump node at the start of the segment where s could be

	// now slow lane
	for (;index < i; index++) {
            tmp = tmp->next_;
	}
    //tmp now points to the node at index i
	return tmp->data_; 
}

string JumpList::print() const {
	Node* current = head_;
	std::string normalNodes, jumpNodes, gapNumbers, result;
    int remainingNodes = size();
	while (current != nullptr){
        
        normalNodes += current->data_;
        if (current->gap_ > 0){
            
            gapNumbers += std::to_string(current->gap_);
            jumpNodes += current->data_;

            remainingNodes -= current->gap_; 
            if (remainingNodes > 0){ //check if it is the last jump node before adding space at the end
                gapNumbers += ' ';
                jumpNodes += ' ';
            }
        }
        current = current->next_;
        if (current != nullptr){ //check if it is the last node before adding space at the end
            normalNodes += ' ';
        }
	}
    result = normalNodes + "\n" + jumpNodes + "\n" + gapNumbers;
	return result;
}

string JumpList::prettyPrint() const {
	Node* current = head_;
	std::string normalNodes, jumpNodes, gapNumbers, result;
    int remainingNodes = size(), fillSpace;

	while (current != nullptr){
        if (current->gap_ > 0){
            if (normalNodes.size()){ //check if a gap is needed i.e. it is not the first gap node
                
                fillSpace = normalNodes.size() - jumpNodes.size(); //gap size for jump nodes
                for (int i = 0; i < fillSpace; i++){ //A for loop to fill the space for the jump nodes
                        jumpNodes += '-';
                }
                jumpNodes.replace(jumpNodes.size() -2, 2, "> ");

                fillSpace = normalNodes.size() - gapNumbers.size(); //gap size for jump numbers
                for (int i = 0; i < fillSpace; i++){ //A for loop to fill the space for the gap numbers
                        gapNumbers += ' ';
                }
            }
            gapNumbers += std::to_string(current->gap_);
            jumpNodes += current->data_;
            
            remainingNodes -= current->gap_;
            if (remainingNodes > 0){ //check if it is the last jump node before adding space at the end
                gapNumbers += ' ';
                jumpNodes += ' ';
            }  
        }
        normalNodes += current->data_;
        current = current->next_;
        if (current != nullptr){ //check if it is the last node before adding space at the end
            normalNodes += " --> ";
        }
	}
    result = normalNodes + "\n" + jumpNodes + "\n" + gapNumbers;
	return result;
}

void JumpList::splitSegment(Node* node, int newGap){
    
    //determine and set the gap of the first jump node
    int gap = newGap % 2 == 0 ? newGap/2 : newGap/2 + 1;
    node->gap_ = gap;
    //search for and set the gap of the second new jump node
    Node* current = node->next_;
    int i = 1;
    while (current != nullptr){
        if (i == gap){
            current->gap_ = newGap - gap;
            node->jump_ = current;
            node = current;
            break;
        }
        current = current->next_; 
        i += 1; 
    }
}

bool JumpList::insert(const string& s) {
    if (find(s)) return false; //Already in the list return false
    if (size() == 0){
        head_ = new Node(s, nullptr, nullptr, 1);
        return true; //insertion to an empty list
    }
    //node to be inserted
	Node* newNode = new Node(s, nullptr, nullptr, 0);

    // moving along the fast lane
	Node* tmp = head_;
	while(tmp->jump_ != nullptr && tmp->jump_->data_ < s){
		tmp = tmp->jump_;
	}
	/* tmp now points to the jump node at the start of the segment where s could be
	now slow lane*/
    Node* prev = nullptr; // a pointer to the previous node
	Node* jumpNode = tmp; //keep track of the current gap node
	while(tmp != nullptr) {
		if (tmp->data_ > s){
            if (tmp == head_){ //check if s should go at the start of the list i.e. the head
                head_ = newNode;
                head_->next_ = tmp;
                head_->gap_ = tmp->gap_;
                head_->jump_ = tmp->jump_;

                //old node now becomes a normal node
                tmp->gap_ = 0; 
                tmp->jump_ = nullptr;

                jumpNode = newNode;
            }else{
                newNode->next_ = tmp;
                /*No need to check if prev is nullptr since the logic
                garuntee it to be pointing to a vlid node in this case*/
                prev->next_ = newNode; //check if s is between two nodes
            }
            break;
		}
        //moving on to the next node
		prev = tmp;
        tmp = tmp->next_;
	}
    
    // No next, insert at the end of the list.
	if (newNode->next_ == nullptr) prev->next_ = newNode;

    int numOfJumps = jumpNode->gap_ + 1; //new gaps

    //now we handle the new gap.
    if (numOfJumps > MAX_GAP_SIZE){//split into two segments
        splitSegment(jumpNode, numOfJumps);
    }else{
        jumpNode->gap_ = numOfJumps; //new gap is within limits
    }

	return true;
}

bool JumpList::erase(const string& s) {
	if (head_ == nullptr) return false; //the list is empty
    
    if (!find(s)) return false; //string not in the list

	if (size() == 1){ //node to erase is the head
        delete head_;
        head_ = nullptr;
        return true; 
	}
    
    // moving along the fast lane
	Node* tmp = head_;
	while(tmp->jump_ != nullptr && tmp->jump_->data_ < s){
		tmp = tmp->jump_;
	}
    
    //tmp now points to the start of the target segment
	Node* jumpNode = tmp; //save the jump node
	Node* prev = nullptr; // a pointer to the previous node

	while(tmp != nullptr) {
		if (tmp->data_ == s){
            if(tmp->gap_ > 0){

                if (tmp == head_){//head transfer
                    head_ = tmp->next_;
                    //if the next node is not a jump node, decrease the head's gap
                    if (tmp->next_ != nullptr && tmp->next_->jump_ == nullptr){
                        head_->gap_ = tmp->gap_ - 1;
                    }
                    jumpNode = head_;
                }else{
                    /*No need to check if prev is nullptr since the logic
                    garuntee it to be pointing to a valid node in this case*/
                    prev->next_ = tmp->next_;

                    jumpNode->gap_ += tmp->gap_ - 1; //merge adjacent segments
                }
                //handle the new gap.
                if (jumpNode->gap_ > MAX_GAP_SIZE){//split into two segments
                    splitSegment(jumpNode, jumpNode->gap_);
                    }

            }else{ //The node to delete is not a gap node
                prev->next_ = tmp->next_;
                jumpNode->gap_ -= 1; //decrease the gap node's jumps by one
            }
            delete tmp;
            break;
		}

		prev = tmp;
        tmp = tmp->next_;
	}

	return true;
}