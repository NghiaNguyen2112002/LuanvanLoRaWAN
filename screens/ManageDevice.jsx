import {View, Text, Image, TouchableOpacity, ScrollView, StyleSheet} from "react-native";
import React from 'react'
import { useNavigation } from '@react-navigation/native';
import { Entypo, FontAwesome5, MaterialIcons, Octicons } from '@expo/vector-icons';



const ManageDevice = () => {
  const device={name: 'Manage',default: false};
  const navigation =useNavigation();
  return (
    <View>
      <View className="w-full bg-sky-500/100 px-4 py-6">
        <View className="jusify-between w-full px-4 py-6">
          <TouchableOpacity onPress={() => navigation.goBack()}>
            <MaterialIcons name="chevron-left" size={32} color={"white"} />
          </TouchableOpacity>
        </View>
      </View>
      <View className="py-6">
        <ScrollView>
          <Device device={device}/>
        </ScrollView>
      </View>
    </View>
  );
}

const Device=({device})=>{
  return (
    <View style={styles.card} className="bg-white pt-4 pb-2 my-1">
      <View className="flex-row items-center ">
        <View className="px-4">
          <Octicons name="dot-fill" size={24} color="#0fc920" />
        </View>
        <Text className="text-primaryText font-bold text-lg">
          {device.name}
        </Text>
        <View
          style={{
            flex: 1,
            flexDirection: "row",
            justifyContent: "flex-end",
          }}
          className="items-center"
        >
          <Text className="text-primaryText texl-xl italic text-gray-400">
            {device.default ? "(Default)" : null}
          </Text>
          <TouchableOpacity className="px-1">
            {device.default ? (
              <FontAwesome5 name="dot-circle" size={20} color="black" />
            ) : (
              <FontAwesome5 name="circle" size={20} color="black" />
            )}
          </TouchableOpacity>
          <TouchableOpacity className="px-2">
            <FontAwesome5 name="trash" size={20} color="red" />
          </TouchableOpacity>
        </View>
      </View>
      <View className="pl-10">
        <Text className="text-gray-400"> Lasted update</Text>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  card: {
    shadowOffset: {width: 1, height: 1},
    shadowColor: "#333",
    shadowOpacity: 0.1
  },
});
export default ManageDevice