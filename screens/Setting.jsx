import { View, Text,Image, TouchableOpacity,ActivityIndicator, FlatList, StyleSheet,TextInput } from 'react-native'
import React, { useContext, useEffect } from 'react'
import { FontAwesome, Ionicons, MaterialIcons ,Feather } from '@expo/vector-icons';
import {useNavigation} from "@react-navigation/native";
import {useDispatch} from "react-redux";
import {firebaseAuth, firestoreDB} from "../config/firebase.config";
import {SET_USER_NULL} from "../context/actions/userAction";
import Modal from "react-native-modal";
import {Switch} from "react-native-switch";
import { useState } from 'react';
import { collection, onSnapshot, query } from 'firebase/firestore';
// import { ActivityIndicator } from 'react-native-web';
import {Dimensions} from "react-native";
import {ListItem} from "react-native-elements";
import {Dropdown} from "react-native-element-dropdown";
import * as Device from "expo-device";
import * as Notifications from "expo-notifications";
import { Platform } from 'react-native';
import { AppContext } from '../context/AppContext';
import {
  Menu,
  MenuOptions,
  MenuOption,
  MenuTrigger,
} from "react-native-popup-menu";
import {MenuProvider} from "react-native-popup-menu";
import {UserTextInput} from "../components";



const fullWidth = Dimensions.get("window").width;


const Setting = () => {
  const dispatch = useDispatch();
  const navigation = useNavigation();
  const Logout = async () => {
    await firebaseAuth.signOut().then(() => {
      dispatch(SET_USER_NULL());
      navigation.navigate("Login");
    });
  };
  return (
    <MenuProvider>
      <View className="flex-1">
        <View className="w-full bg-sky-500/100 px-4 py-8 flex-[0.2] ">
          <View className="w-full items-center justify-between">
            <Text className="text-primaryText text-xl font-semibold text-white">
              Profile
            </Text>
          </View>
        </View>
        <View className="w-full bg-white px-4 py-6 rounded-3xl flex-1 rounded-t-[50px] rounded-b-[50px] -mt-20">
          <View className="w-full px-4 ">
            <View className="w-full px-4  rounded-xl  my-3 flex items-center justify-center">
              <Image
                source={{
                  uri: "https://cdn.iconscout.com/icon/free/png-256/free-avatar-370-456322.png?f=webp",
                }}
                className="w-20 h-20"
                resizeMode="cover"
              />
              <Text className="text-primaryText text-xl font-semibold pt-2"> Võ Hoài Nam</Text>
              <View
                className={`border rounded-md px-4 py-3 justify-center justify-between space-x-4 my-2 border-white bg-white`}
                style={styles.card}
              >
                <Text className="text-primaryText font-semibold text-gray-400 pb-2">
                  Email
                </Text>
                <Text className="text-primaryText italic font-semibold ">
                  hoainam15112002@gmail.com
                </Text>
              </View>
              <View
                className={`border rounded-md px-4 py-3 justify-center justify-between space-x-4 my-2 border-white bg-white`}
                style={styles.card}
              >
                <Text className="text-primaryText font-semibold text-gray-400">
                  Phone
                </Text>
                <Text className="text-primaryText italic  font-semibold">
                  0769387713
                </Text>
              </View>
            </View>
            <TouchableOpacity
              onPress={()=>navigation.navigate("ManageDevice")}
              className="w-full px-4 py-2 rounded-xl bg-green-600 mt-8 mb-2 flex items-center justify-center"
            >
              <Text className="py-2 text-xl font-semibold text-white">
                Mange device
              </Text>
            </TouchableOpacity>
            <TouchableOpacity
              onPress={Logout}
              className="w-full px-4 py-2 rounded-xl bg-red-600 flex items-center justify-center"
            >
              <Text className="py-2 text-xl font-semibold text-white">
                Log out
              </Text>
            </TouchableOpacity>
          </View>
        </View>
      </View>
    </MenuProvider>
  );
}

const styles = StyleSheet.create({
    card: {
      shadowOffset: {width: 1, height: 1},
      shadowColor: "#333",
      shadowOpacity: 0.1,
      width:300,
      height: 70
    },
    container: {
      backgroundColor: "white",
      padding: 16,
    },
    dropdown: {
      width: 200,
      height: 50,
      borderColor: "gray",
      borderWidth: 0,
      borderRadius: 8,
      paddingHorizontal: 8,
      paddingLeft: 16,
    },
    icon: {
      marginRight: 5,
    },
    label: {
      position: "absolute",
      backgroundColor: "white",
      left: 22,
      top: 8,
      zIndex: 999,
      paddingHorizontal: 8,
      fontSize: 14,
    },
    placeholderStyle: {
      fontSize: 16,
    },
    selectedTextStyle: {
      fontSize: 16,
      fontWeight: "bold",
    },
    iconStyle: {
      width: 20,
      height: 20,
    },
    inputSearchStyle: {
      height: 40,
      fontSize: 16,
    },
    item: {
      padding: 17,
      flexDirection: "row",
      justifyContent: "space-between",
      alignItems: "center",
    },
    textItem: {
      flex: 1,
      fontSize: 16,
    },
  });
export default Setting