import {
  View,
  Text,
  Image,
  TouchableOpacity,
  ScrollView,
  ActivityIndicator,
  StyleSheet,
} from "react-native";
import React, {useContext, useEffect, useState} from "react";
import {SafeAreaView} from "react-native-safe-area-context";
import {Logo} from "../assets";
import {FontAwesome, Feather, Entypo} from "@expo/vector-icons";
import { LineChart } from "react-native-gifted-charts";
import { AppContext } from "../context/AppContext";
import {Dropdown} from "react-native-element-dropdown";
import Modal from "react-native-modal";
// import {LineChart,Bar YAxis, XAxis, Grid} from "react-native-svg-charts";


const Chart = () => {
  
  const [isLoading, setisLoading] = useState(true);
  const isLoadingBlock=()=>{
    if(isLoading){
      return (
        <Modal deviceHeight={618}>
          <View className="w-full flex items-center justify-center justify-between">
            <ActivityIndicator size={"large"} color={"white"} />
            <Text className="text-2xl text-base text-white">
              Button is loading ....
            </Text>
          </View>
        </Modal>
      );
    }
  }
  const [value, setValue] = useState(null);
  const [isFocus, setIsFocus] = useState(false);
  const renderLabel = () => {
    if (value || isFocus) {
      return (
        <Text style={[styles.label, isFocus && {color: "red"}]}>
          Minimum rate
        </Text>
      );
    }
    return <Text style={styles.label}>Minimum rate</Text>;
  };
  const renderItem = (item) => {
    return (
      <View style={styles.item}>
        <Text style={styles.itemText}>{item.label}</Text>
        {(item.value === samplingFrequency||item.value === device) && (
          <Entypo name="check" size={24} color="black" />
        )}
      </View>
    );
  };
  const freq = [
    {value: 60000, label: "1 minute"},
    {value: 300000, label: "5 minute"},
    {value: 600000, label: "10 minute"},
    {value: 1800000, label: "30 minute"},
  ];
  const {
    temperature,
    humidity,
    extraHumi,
    extraTemp,
    updateRate,
    samplingFrequency,
    listDevice,
    device,
    updateDevice,lastedUpdate
  } = useContext(AppContext);
  console.log(extraHumi);
  const [tempScale, settempScale] = useState(40);
  useEffect(() => {
    if (temperature && humidity && extraTemp && extraHumi) {
      setTimeout(() => {
        setisLoading(false);
      }, 1000);
    }
  }, [temperature, humidity, extraTemp, extraHumi]);

  return (
    <View className="flex-1 bg-white">
      {/* <SafeAreaView> */}
      {isLoadingBlock()}
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
              // onFocus={() => setIsFocus(true)}
              // onBlur={() => setIsFocus(false)}
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
      <View className="w-full px-4 pt-4">
        <View className="w-full">
          <View className="w-full items-center justify-between px-2">
            <Text className="text-primaryText text-base font-semibold text-xl pb-2">
              CHART
            </Text>
          </View>
          {/* <View style={{flex: 1}}> */}
          <View className="w-full">
            <View style={styles.container}>
              {renderLabel()}
              <Dropdown
                style={styles.dropdown}
                placeholderStyle={styles.placeholderStyle}
                selectedTextStyle={styles.selectedTextStyle}
                inputSearchStyle={styles.inputSearchStyle}
                iconStyle={styles.iconStyle}
                data={freq}
                maxHeight={300}
                labelField="label"
                valueField="value"
                placeholder="Select item"
                searchPlaceholder="Search..."
                onFocus={() => setIsFocus(true)}
                onBlur={() => setIsFocus(false)}
                value={samplingFrequency}
                onChange={(item) => {
                  updateRate(item.value);
                  setIsFocus(false);
                  setisLoading(true);
                }}
                renderItem={renderItem}
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
                <ScrollView style={{marginTop: 50}}>
                  <View className="flex" style={{height: 980}}>
                    <View
                      className="flex items-center pt-2 bg-slate-100"
                      style={styles.card}
                    >
                      <Text className="justify-between py-1 text-primaryText text-base font-semibold">
                        Temperature
                      </Text>
                      <LineChart
                        maxValue={
                          Math.ceil(extraTemp.max) - Math.floor(extraTemp.min)? Math.ceil(extraTemp.max) - Math.floor(extraTemp.min):1
                        }
                        data={[...temperature]}
                        height={250}
                        yAxisOffset={Math.floor(extraTemp.min)}
                        width={280}
                        showVerticalLines
                        spacing={20}
                        // spacing={10}
                        initialSpacing={20}
                        color1="skyblue"
                        textColor1="green"
                        dataPointsHeight={6}
                        dataPointsWidth={6}
                        dataPointsColor1="blue"
                        textFontSize={13}
                        onFocusEnabled
                        scrollToEnd
                        hideDataPoints={
                          samplingFrequency == 1800000 ? true : false
                        }
                        curved={true}
                        areaChart
                        startFillColor1="skyblue"
                        startFillColor2="orange"
                        startOpacity={0.8}
                        endOpacity={0.8}
                        // rotateLabel
                      />
                    </View>
                    <View
                      className="flex items-center pt-2 bg-amber-50"
                      style={styles.card}
                    >
                      <Text className="justify-between py-1 text-primaryText text-base font-semibold">
                        Humidity
                      </Text>
                      <LineChart
                        // maxValue={
                        //   Math.ceil(extraHumi.max) - Math.floor(extraHumi.min)
                        // }
                        // onlyPositive={true}
                        // data={[...humidity]}
                        data={[...humidity]}
                        height={250}
                        // yAxisOffset={Math.floor(extraHumi.min)}
                        // noOfSections={2}
                        // width={Dimensions.get("window").width}
                        width={280}
                        showVerticalLines
                        spacing={40}
                        initialSpacing={20}
                        color1="orange"
                        textColor1="green"
                        dataPointsHeight={6}
                        dataPointsWidth={6}
                        dataPointsColor1="red"
                        // textShiftY={-2}
                        // textShiftX={-5}
                        textFontSize={13}
                        onFocusEnabled
                        scrollToEnd
                        curved
                        // interpolateMissingValues={false}
                        // fromZeroValues={false}
                      />
                    </View>
                  </View>
                </ScrollView>
              </>
            )}
          </View>
          {/* </View> */}
        </View>
      </View>
      {/* </SafeAreaView> */}
    </View>
  );
};



// const styles = StyleSheet.create({
//   container: {
//     backgroundColor: "white",
//     padding: 16,
//   },
  
// });
const styles = StyleSheet.create({
  card: {
    borderRadius: 5,
    elevation: 3,
    height: 350,
    // backgroundColor: "#a2e4fc",
    // shadowOffset: {width: 1, height: 1},
    // shadowColor: "#333",
    // shadowOpacity: 2,
    // shadowRadius: 2,
    // borderColor: "#77A7FF", // Màu viền xanh
    // borderWidth: 2, // Độ dày của viền
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
export default Chart;
// export default BarChartExample;
