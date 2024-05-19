import { View, Text,Image, ActivityIndicator } from 'react-native'
import React, { useLayoutEffect } from 'react'
import { Logo } from '../assets'
import { firebaseAuth, firestoreDB } from '../config/firebase.config'
import { useNavigation } from '@react-navigation/native'
import { doc, getDoc } from 'firebase/firestore'
import { useDispatch } from 'react-redux'
import { SET_USER } from '../context/actions/userAction'

const SplashScreen = () => {
    const navigation=useNavigation();
    const dispatch=useDispatch();

    useLayoutEffect(()=>{
        checkLoggedUser();
    },[])
    const checkLoggedUser=async()=>{
        firebaseAuth.onAuthStateChanged(userCred=>{
            if (userCred?.uid){

                getDoc(doc(firestoreDB, "users", userCred?.uid)).then(
                  (docSnap) => {
                    if (docSnap.exists()) {
                      console.log("User data: ", docSnap.data());
                      dispatch(SET_USER(docSnap.data()));
                    }
                  }
                ).then(()=>{
                    navigation.replace("Home");
                });

            }else{
                navigation.replace("Login")
            }
        })
    }
  return (
    <View className="flex-1 items-center justify-center space-y-4 bg-white">
      <Image source={Logo} className="w-24 h-24" resizeMode="contain" />
      <ActivityIndicator size={"large"} color="green"/>
    </View>
  )
}

export default SplashScreen