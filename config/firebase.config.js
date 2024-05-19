import {getApp,getApps,initializeApp} from "firebase/app"
import {getAuth,initializeAuth, getReactNativePersistence} from "firebase/auth"
import {getFirestore} from "firebase/firestore"
import ReactNativeAsyncStorage from "@react-native-async-storage/async-storage";

const firebaseConfig = {
  apiKey: "AIzaSyC8A9N4YAXrM0-lAmhJl-1uU5MNExeSTFg",
  authDomain: "lvenv-da220.firebaseapp.com",  
  databaseURL:
    "https://lvenv-da220-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "lvenv-da220",
  storageBucket: "lvenv-da220.appspot.com",
  messagingSenderId: "159475053991",
  appId: "1:159475053991:web:3911aa585f3e35fce8f282",
  measurementId: "G-ERSP1CT2D3",
};

const app=getApps.length>0?getApp():initializeApp(firebaseConfig)

// const firebaseAuth=getAuth();
const firebaseAuth = initializeAuth(app, {
  persistence: getReactNativePersistence(ReactNativeAsyncStorage),
});
const firestoreDB=getFirestore(app);

export{app,firestoreDB,firebaseAuth};


