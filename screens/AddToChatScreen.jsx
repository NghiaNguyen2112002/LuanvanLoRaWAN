import { View, Text, TouchableOpacity, Image, TextInput } from 'react-native'
import React, { useState } from 'react'
import { FontAwesome, Ionicons, MaterialIcons } from '@expo/vector-icons';
import { useNavigation } from '@react-navigation/native';
import { Logo } from '../assets';
import { useDispatch, useSelector } from 'react-redux';
import { setDoc,doc } from 'firebase/firestore';
import { firebaseAuth, firestoreDB } from '../config/firebase.config';
import { SET_USER_NULL } from '../context/actions/userAction';

const AddToChatScreen = () => {
    const dispatch=useDispatch()
    const navigation=useNavigation();
    const user=useSelector((state)=>state.user.user)
    const [addChat, setAddChat] = useState("")
    const createNewChat=async()=>{
        let id=`${Date.now()}`; 
        const _doc={
            _id: id,
            user: user,
            chatName: addChat
        }
        if(addChat){
            setDoc(doc(firestoreDB,'chats',id),_doc).then(()=>{
                setAddChat(""); 
                navigation.replace("Home");
            }).catch(err=>{
                alert("Error:",err.message);
            })
        }
    }
    const Logout= async()=>{
        await firebaseAuth.signOut().then(()=>{
            dispatch(SET_USER_NULL);
            navigation.navigate("Login");
        })
    }
  return (
    <View className="flex-1">
      <View className="w-full bg-sky-500/100 px-4 py-12 flex-[0.2]">
        <View className="flex-row items-center justify-between w-full px-4 py-6">
          <TouchableOpacity onPress={() => navigation.goBack()}>
            <MaterialIcons name="chevron-left" size={32} color={"#fbfbfb"} />
          </TouchableOpacity>
          <View className="flex-row items-center justify-between space-x-3">
            <TouchableOpacity className="w-12 h-12 rounded-full border flex items-center justify-center">
              <Image
                source={{uri: "http://"}}
                className="w-12 h-12"
                resizeMode="cover"
              />
            </TouchableOpacity>
          </View>
        </View>
      </View>
      <View className="w-full bg-white px-4 py-6 rounded-3xl flex-1 rounded-t-[50px] -mt-10">
        <View className="w-full px-4 py-4">
          <View className="w-full px-4 flex-row items-center justify-between py-3 rounded-xl border border-gray-200 space-x-3">
            <Ionicons name="chatbubbles" size={24} color={"#777"} />
            <TextInput
              className="flex-1 text-lg text-primaryText -mt-2 h-12 w-full"
              placeholder="Create a chat"
              placeholderTextColor={"#999"}
              value={addChat}
              onChangeText={(text) => setAddChat(text)}
            />
            <TouchableOpacity onPress={createNewChat}>
              <FontAwesome name="send" size={24} color={"#777"} />
            </TouchableOpacity>
            <TouchableOpacity onPress={Logout}>
              <MaterialIcons name="logout" size={24} color="black" />
            </TouchableOpacity>
          </View>
        </View>
      </View>
    </View>
  );
}

export default AddToChatScreen