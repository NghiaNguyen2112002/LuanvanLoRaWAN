import {
  AppRegistry,
  View,
  Text,
  Image,
  TouchableOpacity,
  ScrollView,
  ActivityIndicator,
  StyleSheet,
  processColor,
  FlatList,
  StatusBar
} from "react-native";
import React, { useContext, useEffect, useRef, useState } from 'react'
import { useSelector } from 'react-redux'
import { SafeAreaView } from 'react-native-safe-area-context';
import {Logo} from "../assets"
import {
  FontAwesome,
  FontAwesome5,
  Ionicons,
  MaterialCommunityIcons,
  Entypo,
} from "@expo/vector-icons"; 
import {useNavigation, useRoute} from "@react-navigation/native";
import { Card, Button} from "@rneui/themed";
import { useFonts } from "expo-font";
// import { LineChart } from "react-native-chart-kit";
import {BarChart, LineChart} from "react-native-gifted-charts";
// import {LineChart} from "react-native-charts-wrapper";
// import {LineChart, AreaChart, BarChart, Grid} from "react-native-svg-charts";

// import {Avatar, Card} from "react-native-paper";

import {Dimensions} from "react-native";
import { collection, doc, getDoc, getDocs, limit, onSnapshot, orderBy, query, where } from "firebase/firestore";
import { firestoreDB } from "../config/firebase.config";
import { create } from "@mui/material/styles/createTransitions";
import { color } from "@rneui/base";
import { AppContext } from "../context/AppContext";
// import CircularProgress from "react-native-circular-progress-indicator";
import {DashedCircularIndicator} from "rn-dashed-circular-indicator";
// import Animated from "react-native-reanimated";
import {Dropdown} from "react-native-element-dropdown";
import LinearGradient from "react-native-linear-gradient";


const screenWidth = 220;




function getCurrentDate() {
  const today = new Date();
  let year = today.getFullYear();
  let month = today.getMonth() + 1;
  let day = today.getDate();

  // Thêm số 0 vào trước nếu cần thiết
  if (month < 10) {
    month = "0" + month;
  }
  if (day < 10) {
    day = "0" + day;
  }

  // Định dạng theo yyyy-mm-dd
  const formattedDate = year + "-" + month + "-" + day;

  return formattedDate;
}


const formatDateTime=(timestamp)=>{
    var date = new Date(timestamp)
    var hours = date.getHours();
    var minutes = "0" + date.getMinutes();
    var day = "0" + date.getDate();
    var month = "0" + (date.getMonth() + 1);
    var year = date.getFullYear();

    var formattedTime = hours + ':' + minutes.substr(-2);
    var formattedDate = day.substr(-2) + '/' + month.substr(-2) + '/' + year;

    return formattedTime + ' ' + formattedDate;
}



const HomeScreen = ({navigation}) => {

  const {showData,listDevice,device,updateDevice,lastedUpdate}=useContext(AppContext);
  const [data, setData] = useState({});


  const [isLoading, setisLoading] = useState(true);
  useEffect(() => {
    setTimeout(() => {
      setisLoading(false);
    }, 1000);
  }, []);

  const [value, setValue] = useState(null);
  const [isFocus, setIsFocus] = useState(false);
  const renderItem = (item) => {
    return (
      <View style={styles.item}>
        <Text style={styles.itemText}>{item.label}</Text>
        {( item.value === device) && (
          <Entypo name="check" size={24} color="black" />
        )}
      </View>
    );
  };
  return (
    <View className="flex-1">
      {/* <SafeAreaView> */}
      <View className="w-full flex-row items-center justify-between px-4 pt-8 pb-4 bg-sky-500/100">
        <View
          className="w-full flex-row items-center justify-between px-4 py-2 "
          style={styles.header}
        >
          <View style={styles.container}>
            <Dropdown
              style={styles.dropdownDevice}
              placeholderStyle={styles.placeholderStyle}
              selectedTextStyle={styles.selectedTextStyle}
              inputSearchStyle={styles.inputSearchStyle}
              iconStyle={styles.iconStyle}
              data={listDevice}
              maxHeight={300}
              labelField="label"
              valueField="value"
              placeholder="Select item"
              searchPlaceholder="Search..."
              onFocus={() => setIsFocus(true)}
              onBlur={() => setIsFocus(false)}
              value={device}
              onChange={(item) => {
                updateDevice(item.value);
                setIsFocus(false);
              }}
              renderItem={renderItem}
            />
            <View style={{paddingLeft: 16, paddingBottom: 4}}>
              <Text className="text-primaryText">
                All: {listDevice.length} devices
              </Text>
            </View>
            <View style={{paddingLeft: 16, paddingBottom: 16}}>
              <Text className="text-primaryText">
                Lasted update: {lastedUpdate}
              </Text>
            </View>
          </View>
          <View>
            <Image
              source={{
                uri: "https://cdn.iconscout.com/icon/free/png-256/free-avatar-370-456322.png?f=webp",
              }}
              className="w-12 h-12"
              resizeMode="cover"
            />
          </View>
        </View>
      </View>
      {/* <View className="w-full flex-row items-center justify-between px-4 py-2 ">
          <TouchableOpacity className="w-12 h-12 rounded-full flex items-center justify-center">
            <Image source={Logo} className="w-12 h-12" resizeMode="cover" />
          </TouchableOpacity>
          <Text className="pl-3 text-primaryText text-xl font-semibold">
            Device
          </Text>
          <TouchableOpacity className="w-12 h-12 rounded-full border flex items-center justify-center">
            <Image
              source={{
                uri: "https://t3.ftcdn.net/jpg/05/17/79/88/360_F_517798849_WuXhHTpg2djTbfNf0FQAjzFEoluHpnct.jpg",
              }}
              className="w-12 h-12"
              resizeMode="cover"
            />
          </TouchableOpacity>
        </View> */}
      <View className="w-full">
        <View className="w-full items-center justify-between">
          <Text className="text-primaryText text-base text-xl font-semibold pt-3">
            INFORMATION
          </Text>
        </View>
        {isLoading ? (
          <View
            className="w-full flex items-center justify-center justify-between"
            style={{marginTop: 200}}
          >
            <ActivityIndicator size={"large"} color={"#60a5fa"} />
          </View>
        ) : (
          <>
            <View className="flex justify-between items-center">
              <DashedCircularIndicator
                selectedValue={showData.battery}
                maxValue={100}
                radius={110}
                textColor="#0f4fff"
                activeStrokeColor="#0f4fff"
                withGradient
                label={"Battery"}
              />
            </View>
            <View className="flex justify-between items-center flex-row pl-4 pb-4 ">
              <AQIComponent showData={showData} type={1} />
              <AQIComponent showData={showData} type={2} />
            </View>
          </>
        )}
      </View>
      {/* </SafeAreaView> */}
    </View>
  );
};

  const AQIComponent = ({showData,type}) => {
    return (
      <View
        className="items-center justify-center bg-slate-100"
        style={[styles.card]}
      >
        <View className="items-center justify-between">
          {/* Icon */}
          <View className="h-16 justify-center">
            {type == 1 ? (
              <FontAwesome5 name="thermometer-half" size={30} color="#0176D3" />
            ) : (
              <Ionicons name="water" size={30} color="#0176D3" />
            )}
          </View>
          {/* Show data */}
          <View className="flex-row pb-1">
            <View className="items-center justify-center">
              <Text className="flex text-primaryText text-4xl font-bold justify-between">
                {type == 1 ? showData.temperature : showData.humidity}
              </Text>
            </View>
            <View className="justify-center">
              {type == 1 ? (
                <MaterialCommunityIcons
                  name="temperature-celsius"
                  size={40}
                  color="black"
                />
              ) : (
                <View className="pl-1">
                  <FontAwesome
                    name="percent"
                    className="ml-px"
                    size={30}
                    color="black"
                  />
                </View>
              )}
            </View>
          </View>
          {/* Label */}
          <Text
            className="h-10 text-primaryText text-lg font-semibold justify-center"
            style={{color: "#0176D3"}}
          >
            {type == 1 ? "Temperature" : "Humidity"}
          </Text>
        </View>
      </View>
    );
  };

 

  const styles = StyleSheet.create({
    card: {
      borderRadius: 30,
      elevation: 3,
      width: 150,
      shadowOffset: {width: 1, height: 1},
      shadowColor: "#333",
      shadowOpacity: 2,
      marginRight: 15,
      marginTop: 10,
      marginBottom: 20,
    },
    red: {
      backgroundColor: "#ca1e33",
    },
    blue: {
      backgroundColor: "#1f7ab1",
    },
    container: {
      flex: 1,
      backgroundColor: "#F5FCFF",
    },
    chart: {
      flex: 1,
    },
    container: {
      flex: 1,
      backgroundColor: "white",
    },
    header: {
      borderBottomWidth: 0,
      borderBottomColor: "#ede9e8",
      backgroundColor: "white",
      borderRadius: 10,
      shadowOpacity: 2,
      shadowRadius: 2,
      shadowOffset: {width: 1, height: 1},
      shadowColor: "#333",
    },
    chart: {
      flex: 1,
    },
    dropdown: {
      height: 50,
      borderColor: "gray",
      borderWidth: 0.5,
      borderRadius: 8,
      paddingHorizontal: 8,
      paddingLeft: 16,
      backgroundColor: "white",
    },
    dropdownDevice: {
      width: 200,
      height: 50,
      borderColor: "gray",
      // borderWidth: 0.5,
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
      left: 10,
      top: -10,
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

export default HomeScreen