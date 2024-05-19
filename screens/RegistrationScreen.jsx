import {View, Text, Image, Dimensions, TouchableOpacity} from "react-native";
import React,{useState} from 'react'
import {BG,Logo} from '../assets'
import {UserTextInput} from '../components'
import { useNavigation } from "@react-navigation/native";
import { createUserWithEmailAndPassword } from "firebase/auth";
import { setDoc,doc } from "firebase/firestore";
import { firebaseAuth, firestoreDB } from "../config/firebase.config";




const RegistrationScreen = () => {
  
  const screenWidth=Math.round(Dimensions.get("window").width);
  const [name, setName] = useState("");
  const [email,setEmail]=useState("");
  const [password, setPassword]=useState("")

  const navigation=useNavigation();

  const handleSignUp=async()=>{
    if(email!==""){
      await createUserWithEmailAndPassword(firebaseAuth,email,password).then((userCred)=>{
        if(userCred){
          const data = {
            _id: userCred.user.uid, 
            fullName: name,
            providerData: userCred.user.providerData[0]
          };
          // console.log(data);
          setDoc(doc(firestoreDB,'users', userCred?.user.uid),data).then(()=>{
            navigation.navigate("Login");
          });
        }
        
      });
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
        <Text className="text-primaryText text-xl font-semibold">Register</Text>
        <View className="w-full flex items-center justify-center">
          <UserTextInput
            placeholder="Full Name"
            isPass={false}
            setStateValue={setName}
          />
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
            onPress={handleSignUp}
            className="w-full px-4 py-2 rounded-xl bg-sky-500/100 my-3 flex items-center justify-center"
          >
            <Text className="py-2 text-xl font-semibold">Sign Up</Text>
          </TouchableOpacity>

          <View className="w-full py-4 flex-row items-center justify-center space-x-2">
            <Text className="text-base text-primaryText">Have an account?</Text>
            <TouchableOpacity onPress={() => navigation.navigate("Login")}>
              <Text className="text-base font-semibold text-primaryBold">
                Login Here
              </Text>
            </TouchableOpacity>
          </View>
        </View>
      </View>
    </View>
  );
}

export default RegistrationScreen