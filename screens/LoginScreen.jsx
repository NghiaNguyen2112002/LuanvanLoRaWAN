import {View, Text, Image, Dimensions, TouchableOpacity} from "react-native";
import React,{useState} from 'react'
import {BG,Logo} from '../assets'
import {UserTextInput} from '../components'
import { useNavigation } from "@react-navigation/native";
import { signInWithEmailAndPassword } from "firebase/auth";
import { firebaseAuth, firestoreDB } from "../config/firebase.config";
import { doc, getDoc } from "firebase/firestore";
import { useDispatch } from "react-redux";
import { SET_USER } from "../context/actions/userAction";

const LoginScreen = () => {
  const screenWidth=Math.round(Dimensions.get("window").width);
  const [email,setEmail]=useState("");
  const [password, setPassword]=useState("")

  const navigation=useNavigation();
  const dispatch = useDispatch();

  const handleLogin= async()=>{
    if(email!==""){
      await signInWithEmailAndPassword(firebaseAuth,email,password).then(userCred=>{
        if(userCred){
          getDoc(doc(firestoreDB,'users',userCred?.user.uid)).then(docSnap=>{
            if(docSnap.exists()){
              console.log("User data: ", docSnap.data());
              dispatch(SET_USER(docSnap.data()));
            }
          })
        }
      }).catch(err=>{
        console.log("Error: ", err.message);
      })
    }
  }

  return (
    <View className="flex-1 items-center justify-start">
      <Image
        source={BG}
        resizeMode="cover"
        className="h-96"
        style={{width: screenWidth}}
      />
      <View className="w-full h-full bg-white rounded-tl-[50px] rounded-tr-[50px] -mt-64 items-center justify-start py-6 px-6 space-y-6">
        <Image
          source={Logo}
          resizeMode="contain"
          className="w-16 h-16"
          style={{width: screenWidth}}
        />
        <Text className="py-2 text-primaryText text-xl font-semibold">
          Login
        </Text>
        <View className="w-full flex items-center justify-center">
          <UserTextInput
            placeholder="Email"
            isPass={false}
            setStateValue={setEmail}
          />
          <UserTextInput
            placeholder="Password"
            isPass={true}
            setStateValue={setPassword}
          />
          <TouchableOpacity
            onPress={handleLogin}
            className="w-full px-4 py-2 rounded-xl bg-sky-500/100 my-3 flex items-center justify-center"
          >
            <Text className="py-2 text-xl font-semibold">Sign In</Text>
          </TouchableOpacity>

          <View className="w-full py-4 flex-row items-center justify-center space-x-2">
            <Text className="text-base text-primaryText">
              Don't have an account?
            </Text>
            <TouchableOpacity onPress={() => navigation.navigate("Register")}>
              <Text className="text-base font-semibold text-primaryBold">
                Create Here
              </Text>
            </TouchableOpacity>
          </View>
        </View>
      </View>
    </View>
  );
}

export default LoginScreen