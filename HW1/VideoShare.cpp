#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "VideoShare.hpp"
#include "User.hpp"

using namespace std;

void VideoShare::printAllVideos() {
    videos.print();
}

void VideoShare::printAllUsers() {
    users.print();
}

/* TO-DO: method implementations below */

VideoShare::VideoShare(){
    LinkedList<User> users;
    LinkedList<Video> videos;
}
VideoShare::~VideoShare(){}

void VideoShare::createUser(const string & userName, const string & name, const string & surname){
    User newUser = User (userName, name, surname);
    users.insertNode(users.getHead(), newUser);
}
void VideoShare::createVideo(const string & title, const string & genre){
    Video newVideo = Video(title, genre);
    videos.insertNode(videos.getHead(), newVideo);
}

void VideoShare::loadUsers(const string & fileName){

    ifstream file;
    string line;
    file.open(fileName.c_str());

    while(file.good()){

        getline(file, line);
        size_t pos = line.find(";",0);
        size_t pre_pos = 0;
        if(pos == 0 || pos == string::npos){// NO USERNAME or NOT FOUND!
            continue;
        }
        else{
            string username = line.substr(0,pos);
            pre_pos = pos+1;
            pos = line.find(";",pos+1);
            string name = line.substr(pre_pos, pos-pre_pos);
            string surname = line.substr(pos+1);
            createUser(username,name,surname);
        }
    }
    file.close();
}

void VideoShare::loadVideos(const string & fileName){
    ifstream file;
    string line;
    file.open(fileName.c_str());
    while(file.good()){

        getline(file, line);
        size_t pos = line.find(";",0);
        size_t pre_pos = 0;
        if(pos == 0 || pos == string::npos){// NO TITLE !! INVALID.
            continue;
        }
        else{
            string title = line.substr(0,pos);
            string genre = line.substr(pos+1);
            createVideo(title,genre);
        }
    }
    file.close();
}


void VideoShare::addFriendship(const string & userName1, const string & userName2){
    User* usr1 = (users.findNode(userName1)) -> getDataPtr();
    User* usr2 = (users.findNode(userName2)) -> getDataPtr();
    usr1 ->addFriend(usr2);
    usr2 ->addFriend(usr1);
}
void VideoShare::removeFriendship(const string & userName1, const string & userName2){
    User* usr1 = (users.findNode(userName1)) -> getDataPtr();
    User* usr2 = (users.findNode(userName2)) -> getDataPtr();
    usr1 ->removeFriend(usr2);
    usr2 ->removeFriend(usr1);
}
void VideoShare::updateUserStatus(const string & userName, Status newStatus){
    User* usr = (users.findNode(userName)) -> getDataPtr();
    usr->updateStatus(newStatus);
}
void VideoShare::subscribe(const string & userName, const string & videoTitle){

    if(users.findNode(userName) != NULL){// if the user exists.

        User* usr = (users.findNode(userName))     -> getDataPtr();
        if(usr->getStatus() != SUSPENDED){ //if the user is NOT suspended then we subscribe him.
            Video* vid = (videos.findNode(videoTitle)) -> getDataPtr();
            usr->subscribe(vid);
        }
    }

}
void VideoShare::unSubscribe(const string & userName, const string & videoTitle){
    if(users.findNode(userName) != NULL){ // if the user exists.

        User* usr = (users.findNode(userName))     -> getDataPtr();
        if(usr->getStatus() != SUSPENDED){
            Video* vid = (videos.findNode(videoTitle)) -> getDataPtr();
            usr->unSubscribe(vid);
        }
    }
}
void VideoShare::RemoveMutualFriendship(User* userToBeDeleted){
    //for each of this guys` friends, delete him from their friends list.
    LinkedList<User*> *friendslist = userToBeDeleted->getFriends(); //friends list of "our guy".
    Node<User*> *ptr = friendslist->first();
    while(ptr){
        ptr->getData()->removeFriend(userToBeDeleted);
        ptr = ptr->getNext();
    }

}

void VideoShare::deleteUser(const string & userName){

    Node<User>* tmp = users.findPrev(userName);
    if(tmp){
      RemoveMutualFriendship(tmp->getNext()->getDataPtr());//since we found the previous user,
      //simply the next is the NODE OF our guy.! so also get the DATA which is the actual USER class!
      users.deleteNode(tmp);
    }
}
void VideoShare::printUserSubscriptions(const string & userName){
    Node<User> *tmp= users.findNode(userName);
    if(tmp){
            User* usr = tmp-> getDataPtr();
            usr->printSubscriptions();
    }

}
void VideoShare::printFriendsOfUser(const string & userName){

    if(users.findNode(userName)){
         (users.findNode(userName))->getDataPtr()->printFriends();
    }
   
}
void VideoShare::sortUserSubscriptions(const string & userName){
  Node<User>* userptr = users.findNode(userName);
  if(userptr){// if valid user.
    LinkedList<Video*> *subptr = userptr->getDataPtr()->getSubscriptions();
    bubbleSort(subptr);
  }
}

void VideoShare::bubbleSort(LinkedList<Video*> *ptr){
    bool sorted = false ;
    int n = ptr->getLength();
    if(n < 2){return;} // empty or 1_element(sorted!) list.

    Node<Video*> *nodeptr1 = ptr->first();
    Node<Video*> *nodeptr2 = ptr->first()->getNext();

    for (int i = 0; (i < n-1) && !sorted; i++) {
    sorted = true;

    nodeptr1 = ptr->first();
    nodeptr2 =  ptr->first()->getNext();

    for (int j=1; j <= n-i-1; j++){
      string title1 = nodeptr1->getData()->getTitle();
      string title2 = nodeptr2->getData()->getTitle();
      if (title1 > title2) { //
          swap(nodeptr1->getDataPtr(), nodeptr2->getDataPtr()); // Swap these two
          sorted = false ;// Mark exchange
          nodeptr2 = nodeptr2->getNext();
          nodeptr1 = nodeptr1->getNext();
      }
      else{
         nodeptr2 = nodeptr2->getNext();
         nodeptr1 = nodeptr1->getNext();
        }
    }
  }
}

void VideoShare::swap(Video** vidptr1, Video** vidptr2){
  Video* tmp = *vidptr1;
  *vidptr1 = *vidptr2;
  *vidptr2 = tmp;
}

void VideoShare::printCommonSubscriptions(const string & userName1, const string & userName2){
    Node<User> *tmp1 = users.findNode(userName1);
    Node<User> *tmp2 = users.findNode(userName2);
    if(tmp1 && tmp2){
            User* user1 = tmp1-> getDataPtr();
            User* user2 = tmp2-> getDataPtr();
            LinkedList<Video*> *sub1 = user1->getSubscriptions();
            LinkedList<Video*> *sub2 = user2->getSubscriptions();
            Node<Video*> *ptr1 = sub1 -> first();
            Node<Video*> *ptr2 = sub2 -> first();
            while(ptr1){ // traverse the whole subscriptions list of user1.
                string title1 = ptr1 -> getData()-> getTitle();
                string title2 = ptr2 -> getData()-> getTitle();
                if(title1 == title2){
                    cout << (*(ptr1->getData())); // print the corresponding video entry.
                    ptr1 = ptr1 -> getNext(); // look for other  commons!
                }
                else if(title1 < title2){//since sorted in ascending order, no way to find title1 after this spot.
                    ptr1 = ptr1 -> getNext();
                }
                else{ // title1 > title2. keep searching ! maybe the next one is a match??
                    ptr2 = ptr2 -> getNext();
                }
            }
    }
    cout << endl;
}

void VideoShare::printFriendSubscriptions(const string & userName){
    Node<User> *tmp = users.findNode(userName);
    if(tmp){ // user actually exists.
        LinkedList<User*> *friends = tmp->getDataPtr()->getFriends();
        Node<User*> *friendptr = friends->first();
        LinkedList<Video*> result; // accumulate the videos in this list. insert smartly and you`ll have no duplicates.

        while(friendptr){
            User* myfriend = friendptr->getData();
            LinkedList<Video*> *myfriendsubs = myfriend -> getSubscriptions();
            Node<Video*> *myfriendsubptr = myfriendsubs -> first();

            while(myfriendsubptr){ // traverse this friends` list of subscriptions and add the videos into result list.
                Node<Video*> *resultptr = result.getHead(); // the dummynode of the list that we will insert.
                string title = myfriendsubptr -> getData() -> getTitle();
                Node<Video*> *prev = NULL;
                bool isDuplicate = false;

                while(resultptr->getNext()){ // try to find the correct spot for insertion. THIS is the COSTLY operation.
                    string currentTitle = resultptr->getNext()->getData()->getTitle();
                    if(currentTitle == title){ // duplicate! no insertion.
                        isDuplicate = true;
                        break;
                    }
                    else if(currentTitle > title){ // right spot to insert !
                        prev = resultptr;
                        break;
                    }
                    else{ // means the title is bigger than all those inserted before. insert it at the end then.
                        resultptr = resultptr -> getNext();
                    }
                }
                prev = resultptr; // found the correct spot. which is AFTER the prev.
                if(prev && !isDuplicate)
                {
                    result.insertNode(prev,myfriendsubptr->getData()); //this is O(1) anyway.
                }
                myfriendsubptr = myfriendsubptr->getNext();
            }

            friendptr = friendptr->getNext();
        } // all the insertion to RESULT list is finished. NOW PRINT !
        Node<Video*> *resultptr = result.first();
        while(resultptr){
            cout << *(resultptr->getData()); // print all the video entries. they are already in ascending order.
            resultptr = resultptr->getNext();
        }
    }
    cout<<endl;

}

// LAST EDIT -- LAST EDIT --- //

bool VideoShare::isNotCheckedBefore(LinkedList<string>* list, string &username){ // returns true if user is in the list.
  Node<string>* ptr = list->findNode(username);

  return (ptr == NULL); // if ptr NULL then this user is NOT checked before.
}

bool VideoShare::isConnectedHelper(LinkedList<string>* list, User* currFriend, string key){
  LinkedList<User*> *friendsList = currFriend->getFriends();
  Node<User*> *ptr = friendsList -> first();
  bool linkFound = false;
  while(ptr) // traverse the whole list.
  {
    User* currFriendsFriend = ptr->getData();
    string currName = currFriendsFriend -> getUsername();
    if(isNotCheckedBefore(list, currName)) // if we are visiting this user for the FIRST time.
    {
      if(currName == key) // we found the link!
      {
        return true;
      }
      list->insertNode(list->getHead(), currName); // mark him as checked.
      linkFound =  isConnectedHelper(list,currFriendsFriend, key); // search thru this friends` friends list !
      if(linkFound)// if we found the user2 in this friends` friends list then link is formed. we can terminate.
      {
        return true;
      }
      ptr = ptr->getNext();
    }
    else{
      ptr = ptr->getNext();
    }
  }
  return linkFound;

}
bool VideoShare::isConnected(const string & userName1, const string & userName2){
  // we will start with user1 and try to reach the username2. if we can then it means they are linked by some chain of friends.
    Node<User> *tmp = users.findNode(userName1);

    if(tmp)
    { // user actually exist.
      LinkedList<User*> *friends = tmp->getDataPtr()->getFriends();
      Node<User*> *friendptr = friends->first();
      bool linkFound = false;
      LinkedList<string> checkedFriends; //accumulate checked friends here to avoid looping/recursing forever...
      checkedFriends.insertNode(checkedFriends.getHead(), userName1);
      while(friendptr)
      {
        User* currFriend = friendptr ->getData();
        string currFriendName = currFriend -> getUsername();
        if(isNotCheckedBefore(&checkedFriends, currFriendName)) // if we are visiting this user for the FIRST time.
        {
          if(currFriendName == userName2) // we found the link!
          {
            return true;
          }
          checkedFriends.insertNode(checkedFriends.getHead(), currFriendName); // mark him as checked.
          linkFound =  isConnectedHelper(&checkedFriends,currFriend, userName2); // search thru this friends` friends list !
          if(linkFound)// if we found the user2 in this friends` friends list then link is formed. we can terminate.
          {
            return true;
          }
          friendptr = friendptr->getNext();
        }
        else{
          friendptr = friendptr->getNext();
        }
      }
      return linkFound; // we could just say false? cuz if lnk wasfound we could have terminated early anyway.
    }
}
