import {View, Text, Image, TouchableOpacity, ScrollView, ActivityIndicator, Button,StyleSheet, Dimensions} from "react-native";
import React, { useContext, useEffect, useState } from 'react'
import { useSelector } from 'react-redux'
import { SafeAreaView } from 'react-native-safe-area-context';
import {Logo} from "../assets"
import { FontAwesome, Ionicons,Entypo } from "@expo/vector-icons";
import { useNavigation } from "@react-navigation/native";
import { collection, onSnapshot, query } from "firebase/firestore";
import { firestoreDB } from "../config/firebase.config";
import { Switch } from "react-native-switch";
import Modal from "react-native-modal";
import * as Device from "expo-device";
import * as Notifications from "expo-notifications";
import {Platform} from "react-native";
import {Dropdown} from "react-native-element-dropdown";
import { AppContext, AppProvider } from "../context/AppContext";
import { btoa,Buffer } from "buffer";


const fullWidth = Dimensions.get("window").width;

function hexToBase64(hexString) {
  // Convert hexadecimal string to byte array
  let bytes = [];
  for (let i = 0; i < hexString.length; i += 2) {
    bytes.push(parseInt(hexString.substr(i, 2), 16));
  }

  // Convert byte array to base64 string
  let base64String;
  if (typeof window !== "undefined") {
    // Browser environment
    base64String = btoa(String.fromCharCode.apply(null, bytes));
  } else {
    // Node.js environment
    base64String = Buffer.from(bytes).toString("base64");
  }
  return base64String;
}




const Control = () => {
    const navigation = useNavigation();
    const [isLoading, setisLoading] = useState(true);

    const [isLoadingRelay, setisLoadingRelay] = useState(false);
    const [Relay1, setRelay1] = useState(false);
    const [Relay2, setRelay2] = useState(false);
    const [Relay3, setRelay3] = useState(false);
    const [Relay4, setRelay4] = useState(false);
    const [timeoutId, setTimeoutId] = useState(null);
    const [buttonPress, setbuttonPress] = useState();
    const [sendSuccess, setsendSuccess] = useState(false);
    const [uplink, setuplink] = useState(false);


    
    useEffect(() => {
      const q = query(collection(firestoreDB, "relay"));
      const unsuscribe = onSnapshot(q, (snapshot) => {
        snapshot.docChanges().forEach((change) => {
          if (change.type === "modified") {
            setRelay1(change.doc.data().RL1); 
            setRelay2(change.doc.data().RL2);
            setRelay3(change.doc.data().RL3);
            setRelay4(change.doc.data().RL4);
            if (buttonPress == 1) sendNotification(buttonPress, !Relay1, true);
            else if (buttonPress == 2) sendNotification(buttonPress, !Relay2, true);
            else if (buttonPress == 3) sendNotification(buttonPress, !Relay3, true);
            else if (buttonPress == 4) sendNotification(buttonPress, !Relay4, true);
            setsendSuccess(true);
            clearTimeout(timeoutId);
            const id = setTimeout(() => {
              setisLoadingRelay(false);
            }, 5000);
            // setsendSuccess(false);
            // setisLoadingRelay(false);
          } else {
            console.log("log else");
            setRelay1(change.doc.data().RL1);
            setRelay2(change.doc.data().RL2);
            setRelay3(change.doc.data().RL3);
            setRelay4(change.doc.data().RL4);
          }
        });
        setisLoading(false);
      });
      return () => {
        unsuscribe();
      };
    }, uplink?[buttonPress,timeoutId,Relay1,Relay2,Relay3,Relay4]:[]);
    
    const handleButton = async (relay, status) => {
      // setbuttonPress(relay);
      // console.log(buttonPress);
      var payload="";
      // if (relay == 1) {
      //   if (status === "OFF") payload = "oQ==";
      //   else payload = "sQ==";
      // } else if (relay == 2) {
      //   if (status === "OFF") payload = "og==";
      //   else payload = "sg==";
      // } else if (relay == 3) {
      //   if (status === "OFF") payload = "ow==";
      //   else payload = "sw==";
      // } else if (relay == 4) {
      //   if (status === "OFF") payload = "pA==";
      //   else payload = "tA==";
      // }
      
      if (relay == 1) {
        Relay1 ? (payload += "00") : (payload += "01");
        !Relay2 ? (payload += "00") : (payload += "01");
        !Relay3 ? (payload += "00") : (payload += "01");
        !Relay4 ? (payload += "00") : (payload += "01");
        !uplink ? (payload += "00") : (payload += "01");
      } else if (relay == 2) {
        !Relay1 ? (payload += "00") : (payload += "01");
        Relay2 ? (payload += "00") : (payload += "01");
        !Relay3 ? (payload += "00") : (payload += "01");
        !Relay4 ? (payload += "00") : (payload += "01");
        !uplink ? (payload += "00") : (payload += "01");
      } else if (relay == 3) {
        !Relay1 ? (payload += "00") : (payload += "01");
        !Relay2 ? (payload += "00") : (payload += "01");
        Relay3 ? (payload += "00") : (payload += "01");
        !Relay4 ? (payload += "00") : (payload += "01");
        !uplink ? (payload += "00") : (payload += "01");
      } else if (relay == 4) {
        !Relay1 ? (payload += "00") : (payload += "01");
        !Relay2 ? (payload += "00") : (payload += "01");
        !Relay3 ? (payload += "00") : (payload += "01");
        Relay4 ? (payload += "00") : (payload += "01");
        !uplink ? (payload += "00") : (payload += "01");
      }

      const Buffer = require("buffer").Buffer;
      let buffer = Buffer.from(payload, "hex");
      let base64String = buffer.toString("base64");
      try {
        const requestData = {
          downlinks: [
            {
              f_port: 1,
              frm_payload: base64String,
            },
          ],
        };

        const response = await fetch(
          "https://eu1.cloud.thethings.network/api/v3/as/applications/test-atom-eu1/devices/node-05-04-otta/down/replace",
          {
            method: "POST",
            headers: {
              "Content-Type": "application/json",
              Authorization: `Bearer NNSXS.PAMWVMNQ6WJZ5LBZYUI77YIUGCLWK6HPO2LC6MQ.KVTF3JWV5KMTIRBKAIIANKXQXRWM5DXFVPHJS5ZPXFK75OQRQGCQ`,
            },
            body: JSON.stringify(requestData),
          }
        );

        if (!response.ok) {
          throw new Error("Network response was not ok");
        }
      } catch (error) {
        console.error("API Error:", error);
      }
    };
    
    const [expoPushToken, setExpoPushToken] = useState("");

    const sendNotification = async (relay,on,success) => {
      // console.log(on);
      var title=success?"✅":"❌";
      title+=" RELAY "+relay+" ";
      if(success) title += on ? "ON" : "OFF";
      else  title+=on?"OFF":"ON";
      title+=success?" SUCCESSED":" FAILED";
      const message = {
        to: expoPushToken,
        sound: "default",
        title: title,
        badge: 0,
      };

      await fetch("https://exp.host/--/api/v2/push/send", {
        method: "POST",
          headers: {
            host: "exp.host",
            accept: "application/json",
            "accept-encoding": "gzip, deflate",
            "content-type": "application/json",
          },
          body: JSON.stringify(message),
      });
    };

    useEffect(() => {
      registerForPushNotificationsAsync().then((token) => {
        setExpoPushToken(token);
      });
    }, []);

    async function registerForPushNotificationsAsync() {
      let token;

      if (Platform.OS === "android") {
        await Notifications.setNotificationChannelAsync("default", {
          name: "default",
          importance: Notifications.AndroidImportance.MAX,
          vibrationPattern: [0, 250, 250, 250],
          lightColor: "#FF231F7C",
        });
      }

      if (Device.isDevice) {
        const {status: existingStatus} =
          await Notifications.getPermissionsAsync();
        let finalStatus = existingStatus;
        if (existingStatus !== "granted") {
          const {status} = await Notifications.requestPermissionsAsync();
          finalStatus = status;
        }
        if (finalStatus !== "granted") {
          alert("Failed to get push token for push notification!");
          return;
        }
        // Learn more about projectId:
        // https://docs.expo.dev/push-notifications/push-notifications-setup/#configure-projectid
        token = (
          await Notifications.getExpoPushTokenAsync({
            projectId: "77e5a2df-5d00-47c4-b7b3-24ab46069128",
          })
        ).data;
        console.log(token);
      } else {
        alert("Must use physical device for Push Notifications");
      }

      return token;
    }
   
    const [value, setValue] = useState(null);
    const [isFocus, setIsFocus] = useState(false);
    const renderLabel = () => {
      if (value || isFocus) {
        return (
          <Text style={[styles.label, isFocus && {color: "blue"}]}>
            List devices
          </Text>
        );
      }
      return <Text style={styles.label}>Minimum rate</Text>;
    };
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
    const {listDevice,device,lastedUpdate,updateDevice} = useContext(AppContext);
  return (
    <View className="bg-white flex-1">
      <Modal
        isVisible={isLoadingRelay}
        coverScreen={false}
        deviceWidth={fullWidth}
        deviceHeight={618}
      >
        <View className="w-full flex items-center justify-center justify-between">
          <ActivityIndicator size={"large"} color={"white"} />
          <Text className="text-2xl text-base text-white">
            Button is loading ....
          </Text>
        </View>
      </Modal>
      {/* <SafeAreaView> */}
      <View className="w-full items-center justify-between px-4 pt-8 pb-4 bg-sky-500/100">
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
      <ScrollView className="w-full px-4 pt-4">
        <View className="w-full">
          <View className="w-full items-center justify-between px-2">
            <Text className="text-primaryText text-base text-xl font-semibold py-3">
              RELAYS
            </Text>
            <Switch
              value={uplink}
              onValueChange={() => {  
                setuplink(!uplink);
              }}
              inActiveText={"On"}
              circleSize={50}
              circleBorderWidth={2}
              backgroundActive={"blue"}
              backgroundInactive={"gray"}
              changeValueImmediately={true}
              innerCircleStyle={{
                alignItems: "center",
                justifyContent: "center",
              }}
              outerCircleStyle={{}}
              renderActiveText={true}
              renderInActiveText={true}
              switchLeftPx={3}
              switchRightPx={3}
              switchBorderRadius={30}
            />
          </View>
          {isLoading ? (
            <View
              className="w-full flex items-center justify-center"
              style={{marginTop: 200}}
            >
              <ActivityIndicator size={"large"} color={"#60a5fa"} />
            </View>
          ) : (
            <>
              <View className="flex-row ">
                {/* Relay1 */}
                <View
                  className="items-center justify-center bg-slate-100"
                  style={styles.card}
                >
                  <Text className="flex text-primaryText text-2xl font-bold justify-between py-4">
                    Relay 1
                  </Text>
                  <View className="pb-4">
                    <Switch
                      value={Relay1}
                      onValueChange={() => {
                        if(uplink){
                          setisLoadingRelay(true);
                          setsendSuccess(false);
                          Relay1 ? handleButton(1, "OFF") : handleButton(1, "ON");
                          const id = setTimeout(() => {
                            setisLoadingRelay(false);
                            if (!sendSuccess) sendNotification(1, Relay1, false);
                          }, 10000);
                          setTimeoutId(id);
                        }
                        else{
                          handleButton(1, "ON");
                          setRelay1(!Relay1);
                        }
                      }}
                      inActiveText={"Off"}
                      circleSize={50}
                      circleBorderWidth={2}
                      backgroundActive={"green"}
                      backgroundInactive={"gray"}
                      changeValueImmediately={true}
                      innerCircleStyle={{
                        alignItems: "center",
                        justifyContent: "center",
                      }}
                      outerCircleStyle={{}}
                      renderActiveText={true}
                      renderInActiveText={true}
                      switchLeftPx={3}
                      switchRightPx={3}
                      switchBorderRadius={30}
                    />
                  </View>
                </View>
                {/* Relay2 */}
                <View
                  className="items-center justify-center bg-slate-100"
                  style={styles.card}
                >
                  <Text className="flex text-primaryText text-2xl font-bold justify-between py-4">
                    Relay 2
                  </Text>
                  <View className="pb-4">
                    <Switch
                      value={Relay2}
                      onValueChange={() => {
                        if (uplink) {
                          setisLoadingRelay(true);
                          setsendSuccess(false);
                          Relay2
                            ? handleButton(2, "OFF")
                            : handleButton(2, "ON");
                          const id = setTimeout(() => {
                            setisLoadingRelay(false);
                            if (!sendSuccess)
                              sendNotification(2, Relay2, false);
                          }, 10000);
                          setTimeoutId(id);
                        } else {
                          handleButton(2, "ON");
                          setRelay2(!Relay2);
                        }
                      }}
                      inActiveText={"Off"}
                      circleSize={50}
                      circleBorderWidth={2}
                      backgroundActive={"green"}
                      backgroundInactive={"gray"}
                      changeValueImmediately={true}
                      innerCircleStyle={{
                        alignItems: "center",
                        justifyContent: "center",
                      }}
                      outerCircleStyle={{}}
                      renderActiveText={true}
                      renderInActiveText={true}
                      switchLeftPx={3}
                      switchRightPx={3}
                      switchBorderRadius={30}
                    />
                  </View>
                </View>
              </View>
              <View className="flex-row ">
                {/* Relay3 */}
                <View
                  className="items-center justify-center bg-slate-100"
                  style={styles.card}
                >
                  <Text className="flex text-primaryText text-2xl font-bold justify-between py-4">
                    Relay 3
                  </Text>
                  <View className="pb-4">
                    <Switch
                      value={Relay3}
                      onValueChange={() => {
                        if(uplink){
                          setsendSuccess(false);
                          setisLoadingRelay(true);
                          Relay3 ? handleButton(3, "OFF") : handleButton(3, "ON");
                          const id = setTimeout(() => {
                            setisLoadingRelay(false);
                            if (!sendSuccess) sendNotification(3, Relay3, false);
                          }, 10000);
                          setTimeoutId(id);
                        }
                        else{
                          handleButton(3, "ON");
                          setRelay3(!Relay3);
                        }
                      }}
                      inActiveText={"Off"}
                      circleSize={50}
                      circleBorderWidth={2}
                      backgroundActive={"green"}
                      backgroundInactive={"gray"}
                      changeValueImmediately={true}
                      innerCircleStyle={{
                        alignItems: "center",
                        justifyContent: "center",
                      }}
                      outerCircleStyle={{}}
                      renderActiveText={true}
                      renderInActiveText={true}
                      switchLeftPx={3}
                      switchRightPx={3}
                      switchBorderRadius={30}
                    />
                  </View>
                </View>
                {/* Relay4 */}
                <View
                  className="items-center justify-center bg-slate-100"
                  style={styles.card}
                >
                  <Text className="flex text-primaryText text-2xl font-bold justify-between py-4">
                    Relay 4
                  </Text>
                  <View className="pb-4">
                    <Switch
                      value={Relay4}
                      onValueChange={() => {
                        if(uplink){
                          setisLoadingRelay(true);
                          setsendSuccess(false);
                          Relay4 ? handleButton(4, "OFF") : handleButton(4, "ON");
                          const id = setTimeout(() => {
                            setisLoadingRelay(false);
                            if (!sendSuccess) sendNotification(4, Relay4, false);
                          }, 10000);
                          setTimeoutId(id);
                        }
                        else{
                          handleButton(4, "ON");
                          setRelay4(!Relay4);
                        }
                      }}
                      inActiveText={"Off"}
                      circleSize={50}
                      circleBorderWidth={2}
                      backgroundActive={"green"}
                      backgroundInactive={"gray"}
                      changeValueImmediately={true}
                      innerCircleStyle={{
                        alignItems: "center",
                        justifyContent: "center",
                      }}
                      outerCircleStyle={{}}
                      renderActiveText={true}
                      renderInActiveText={true}
                      switchLeftPx={3}
                      switchRightPx={3}
                      switchBorderRadius={30}
                    />
                  </View>
                </View>
              </View>
            </>
          )}
        </View>
      </ScrollView>
      {/* </SafeAreaView> */}
    </View>
  );
}

const styles = StyleSheet.create({
  card: {
    borderRadius: 30,
    elevation: 3,
    width: 150,
    // backgroundColor: "#a2e4fc",
    shadowOffset: {width: 1, height: 1},
    shadowColor: "#333",
    shadowOpacity: 2,
    // shadowRadius: 2,
    // borderColor: "#77A7FF", // Màu viền xanh
    // borderWidth: 2, // Độ dày của viền
    marginRight: 35,
    marginTop: 10,
    marginBottom: 20,
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
export default Control