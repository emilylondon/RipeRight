import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View } from 'react-native';
import React, { useState } from "react";
import DropDownPicker from 'react-native-dropdown-picker';




const App = () => {
  const [open, setOpen] = useState(false);//whether the menu is open or not
  const [value, setValue] = useState(null); //actual fruit being chosen
  const [items, setItems] = useState([
    {label: 'Orange', value: 'orange'},
    {label: 'Apple', value: 'apple'},
    {label: 'Banana', value: 'banana'}
  ]);
  return (
    <View style={styles.container}>
        <Text> Fruit Ripeness Checker!</Text>
        <DropDownPicker
          open={open}
          value={value}
          items={items}
          setOpen={setOpen}
          setValue={setValue}
          setItems={setItems}
        />
        <Text>Your selected fruit: {value}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
});

export default App;