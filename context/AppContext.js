import { createContext, useEffect, useState } from "react";
import {
  collection,
  doc,
  getDoc,
  onSnapshot,
  orderBy,
  query,
  where,
} from "firebase/firestore";
import {firestoreDB} from "../config/firebase.config";

export const AppContext = createContext();
function formatTimestamp(timestamp) {
  // Tạo một đối tượng Date từ timestamp
  const date = new Date(timestamp);

  // Lấy giờ và phút
  const hours = date.getHours().toString().padStart(2, "0");
  const minutes = date.getMinutes().toString().padStart(2, "0");

  // Lấy ngày, tháng và năm
  const day = date.getDate().toString().padStart(2, "0");
  const month = (date.getMonth() + 1).toString().padStart(2, "0"); // Tháng bắt đầu từ 0
  const year = date.getFullYear();

  // Trả về chuỗi đã định dạng
  return `${hours}:${minutes} - ${day}/${month}/${year}`;
}



export const AppProvider = ({children})=>{
    // const startOfDay = new Date();
    const startOfDay = new Date(2024,2,17);
    startOfDay.setHours(0, 0, 0, 0);
    console.log(startOfDay.getTime());
    const [temperature, settemperature] = useState([]);
    const [humidity, sethumidity] = useState([]);
    const [showData, setshowData] = useState({});
    const [lastPoint, setlastPoint] = useState(startOfDay.getTime());
    const [extraTemp, setextraTemp] = useState({min: 0, max: 90, avg: 0});
    const [extraHumi, setextraHumi] = useState({min: 0, max: 90, avg: 0});
    const [listDevice, setlistDevice] = useState([]);
    const [device, setdevice] = useState();
    const [samplingFrequency, setsamplingFrequency] = useState(300000);
    const [lastedUpdate, setlastedUpdate] = useState(formatTimestamp(startOfDay.getTime()));

    const updateDevice=newDevice=>{
      setdevice(newDevice);
    }
    const updateRate=rate=>{
      setsamplingFrequency(rate);
    }

    const getCurrentDate=()=> {
      const today = new Date();
      let year = today.getFullYear();
      let month = today.getMonth() + 1;
      let day = today.getDate();

      if (month < 10) {
        month = "0" + month;
      }
      if (day < 10) {
        day = "0" + day;
      }
      const formattedDate = year + "-" + month + "-" + day+"_"+device;
      return formattedDate;
    }

    useEffect(() => {
      var isFirstLoad=true;
      var tempArray = [];
      var humiArray = [];
      var tempRecord = [];
      var humiRecord = [];
      var lastPointTemp = startOfDay.getTime();
      var showDataTemp = {};  
      var interruptLoop = true;
      var updateLastPoint=false;

      const q = query(
        collection(firestoreDB, getCurrentDate()),
        // collection(firestoreDB, "2024-03-17"),
        orderBy("timeBySecond", "asc"),
        where("timeBySecond", ">=", startOfDay.getTime())
      );
      const unsuscribe = onSnapshot(q, (snapshot) => {
        console.log(samplingFrequency);
        snapshot.docChanges().forEach((change) => {
          console.log(change);
          interruptLoop = true;
          var date = new Date(change.doc.data().timeBySecond);
          var hours = date.getHours();
          if (change.type === "added"&&!isFirstLoad) {
            console.log(lastPointTemp);
            // Tính trung bình với mảng temp Record
            if(change.doc.data().timeBySecond - lastPointTemp < samplingFrequency){
              tempRecord.push({
                value: change.doc.data().temperature,
                dataPointText: String(change.doc.data().temperature),
              });
              humiRecord.push({
                value: change.doc.data().humidity,
                dataPointText: String(change.doc.data().humidity),
              });
              updateLastPoint=true;
            }
            else{
              while(lastPointTemp+samplingFrequency<change.doc.data().timeBySecond){
                tempArray.push({
                  value: null,
                  label: lastPointTemp % 3600000 ? null : hours,
                });
                humiArray.push({
                  value: null,
                  label: lastPointTemp % 3600000 ? null : hours,
                });
                tempRecord=[];
                humiRecord=[];
                lastPointTemp+=samplingFrequency;
              }
              tempRecord.push({
                value: change.doc.data().temperature,
                dataPointText: String(change.doc.data().temperature),
              });
              humiRecord.push({
                value: change.doc.data().humidity,
                dataPointText: String(change.doc.data().humidity),
              });
            }
            showDataTemp = change.doc.data();
          } 
          else {
            // while (lastPointTemp < change.doc.data().timeBySecond) {
            while (interruptLoop) {
              if (change.doc.data().timeBySecond - lastPointTemp < samplingFrequency) {
                interruptLoop=false;
                tempRecord.push({
                  value: change.doc.data().temperature,
                  dataPointText: String(change.doc.data().temperature)
                });
                humiRecord.push({
                  value: change.doc.data().humidity,
                  dataPointText: String(change.doc.data().humidity)
                });
              } else {
                interruptLoop = true;
                // Xét số record trong temp/humi record
                if(tempRecord.length){
                  const tempSum = tempRecord.reduce((acc, curr) => acc + curr.value, 0);
                  const tempAverage = tempSum / tempRecord.length;
                  tempArray.push({
                    value: tempAverage,
                    dataPointText: String(tempAverage),
                    label: lastPointTemp % 3600000 ? null : hours
                  });
                  tempRecord=[];

                  const humiSum = humiRecord.reduce((acc, curr) => acc + curr.value, 0);
                  const humiAverage = humiSum / humiRecord.length;
                  humiArray.push({
                    value: humiAverage,
                    dataPointText: String(humiAverage),
                    label: lastPointTemp % 3600000 ? null : hours
                  });
                  humiRecord=[];
                }
                else if(tempArray.length){
                  tempArray.push({
                    value: null,
                    label: lastPointTemp % 3600000 ? null : hours
                  });
                  humiArray.push({
                    value: null,
                    label: lastPointTemp % 3600000 ? null : hours
                  });
                }
                lastPointTemp += samplingFrequency;
              }
            }
            showDataTemp = change.doc.data();
            // isFirstLoad=false;
          }
        });
        if(tempRecord.length){
          const tempSum = tempRecord.reduce((acc, curr) => acc + curr.value, 0);
          const tempAverage = tempSum / tempRecord.length;
          if(updateLastPoint){
            tempArray.pop();
            humiArray.pop();
            updateLastPoint=false;
            console.log("Delete");
            console.log(tempArray);
          }
          tempArray.push({
            value: tempAverage,
            dataPointText: String(tempAverage),
            // label: lastPointTemp % 3600000 ? null : hours + "h",
          });

          const humiSum = humiRecord.reduce((acc, curr) => acc + curr.value, 0);
          const humiAverage = humiSum / humiRecord.length;
          humiArray.push({
            value: humiAverage,
            dataPointText: String(humiAverage),
            // label: lastPointTemp % 3600000 ? null : hours + "h",
          });
        }
        ///set state
        isFirstLoad=false;
        console.log("ArrayHumi:",humiArray);
        settemperature(tempArray);
        const updatedHumidityArray = [...humiArray];
        sethumidity(humiArray);
        setshowData(showDataTemp);
        setlastPoint(lastPointTemp);
        setlastedUpdate(formatTimestamp(showDataTemp.timeBySecond));
        if(humiArray.length){
          const filteredValuesTemp = tempArray
            .filter((temp) => temp.value !== null)
            .map((temp) => temp.value);
          const maxTemp = Math.max(...filteredValuesTemp);
          const minTemp = Math.min(...filteredValuesTemp);
          const avgTemp =
            filteredValuesTemp.reduce((acc, curr) => acc + curr, 0) /
            filteredValuesTemp.length;
          setextraTemp({
            min: minTemp,
            max: maxTemp,
            avg: avgTemp,
          });
          const filteredValuesHumi = humiArray
            .filter((temp) => temp.value !== null)
            .map((temp) => temp.value);
          const maxHumi = Math.max(...filteredValuesHumi);
          const minHumi = Math.min(...filteredValuesHumi);
          const avgHumi =
            filteredValuesHumi.reduce((acc, curr) => acc + curr, 0) /
            filteredValuesHumi.length;
          setextraHumi({
            min: minHumi,
            max: maxHumi,
            avg: avgHumi,
          });
        }else{
          setextraTemp({
            min: 0,
            max: 90,
            avg: 0,
          });
          setextraHumi({
            min: 0,
            max: 90,
            avg: 0,
          });
        }
      });
      return () => {
        unsuscribe();
      };
    }, [device,samplingFrequency]);
    useEffect(() => {
      const listDevices = onSnapshot(
        query(
          collection(firestoreDB, 'devices')  
        ),
        (snapshot) => {
          snapshot.docChanges().forEach((change) => {
            if(change.doc.data().default) setdevice(change.doc.data().name); 
            setlistDevice((data)=>[...data,{label:change.doc.data().name,value: change.doc.data().name}]);
          })
        }
      );
      return () => {
        listDevices();
      };
    }, []);
    
    return (
      <AppContext.Provider
        value={{
          showData,
          humidity,
          temperature,
          extraTemp,
          extraHumi,
          device,
          listDevice,
          updateDevice,
          updateRate,
          samplingFrequency,
          lastedUpdate
        }}
      >
        {children}
      </AppContext.Provider>
    );
}