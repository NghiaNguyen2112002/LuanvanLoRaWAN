import React, {useEffect, useState} from "react";
import {createBottomTabNavigator} from "@react-navigation/bottom-tabs";
import { Ionicons} from "@expo/vector-icons";
import {
  collection,
  onSnapshot,
  orderBy,
  query,
  where,
} from "firebase/firestore";
import {firestoreDB} from "../config/firebase.config";
import HomeScreen from "./HomeScreen";
import Control from "./Control";
import Chart from "./Chart";
import { AppProvider } from "../context/AppContext";
import Setting from "./Setting";

const Tab = createBottomTabNavigator();

const Test = () => {
  return (
    <AppProvider>
      <Tab.Navigator
        screenOptions={({route}) => ({
          headerShown: false,
          tabBarIcon: ({focused, color, size}) => {
            let iconName;
            if (route.name === "Monitor") {
              iconName = focused ? "bar-chart" : "bar-chart-outline";
            } else if (route.name === "Control") {
              iconName = focused ? "grid" : "grid-outline";
            } else if (route.name === "Chart") {
              iconName = focused ? "analytics" : "analytics-outline";
            } else if (route.name === "Setting") {
              iconName = focused ? "settings" : "settings-outline";
            }
            return <Ionicons name={iconName} size={size} color={color} />;
          },
          tabBarActiveTintColor: "#60a5fa",
          tabBarInactiveTintColor: "gray",
        })}
      >
        <Tab.Screen name="Monitor" component={HomeScreen} />
        <Tab.Screen name="Chart" component={Chart} />
        <Tab.Screen name="Control" component={Control} />
        <Tab.Screen name="Setting" component={Setting} />
      </Tab.Navigator>
    </AppProvider>
  );
};

export default Test;
