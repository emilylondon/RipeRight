import React, { useEffect, useState } from 'react';
import { StyleSheet, Text } from 'react-native';
import { Characteristic } from 'react-native-ble-plx';
import { TouchableOpacity } from 'react-native-gesture-handler';
import { Base64 } from '../lib/base64';

type CharacteristicCardProps = {
  char: Characteristic;
};

const decodeBleString = (value: string | undefined | null): string => {
  if (!value) {
    return '';
  }
  return Base64.decode(value).charCodeAt(0);
};

const CharacteristicCard = ({ char }: CharacteristicCardProps) => {
  const [measure, setMeasure] = useState('');
  const [descriptor, setDescriptor] = useState<string | null>('');

  useEffect(() => {
    // discover characteristic descriptors
    console.log("Setting descriptors: ", char.descriptors);
    char.descriptors().then((desc) => {
      desc[0]?.read().then((val) => {
        if (val) {
          console.log("Value:", val.value);
          console.log("Val:", val);
          setDescriptor(Base64.decode(val.value));
        }
      });
    });

    // read on the characteristic 👏
    char.monitor((err, cha) => {
      if (err) {
        console.log(err)
        console.warn('ERROR');
        return;
      }
      // each received value has to be decoded with a Base64 algorythm you can find on the Internet (or in my repository 😉)
      console.log("About to set measure to ", decodeBleString(cha?.value));
      setMeasure(decodeBleString(cha?.value));
    });
  }, [char]);

  // write on a charactestic the number 6 (e.g.)
  const writeCharacteristic = () => {
    // encode the string with the Base64 algorythm
    char
      .writeWithResponse(Base64.encode('6'))
      .then(() => {
        console.warn('Success');
      })
      .catch((e) => console.log('Error', e));
  };  
  console.log("Measure: " + measure);
  console.log("Descriptor: " + descriptor);
  return (
    <TouchableOpacity
      key={char.uuid}
      style={styles.container}
      onPress={writeCharacteristic}>
      <Text style={styles.measure}>{`Measure : ${measure}`}</Text>
      <Text style={styles.descriptor}>{`Descriptor : ${descriptor}`}</Text>
      <Text>{`UUID : ${char.uuid}`}</Text>
      <Text>{`serviceID : ${char.serviceID}`}</Text>
      <Text>{`serviceUUID : ${char.uuid}`}</Text>
      <Text>{`deviceID : ${char.deviceID}`}</Text>
      <Text>{`isReadable : ${char.isReadable}`}</Text>
      {/* <Text>{`isWritableWithResponse : ${char.isWritableWithResponse}`}</Text>
      <Text>{`isWritableWithoutResponse : ${char.isWritableWithoutResponse}`}</Text> */}
      <Text>{`isNotifiable : ${char.isNotifiable}`}</Text>
      <Text>{`isNotifying : ${char.isNotifying}`}</Text>
      <Text>{`isIndicatable : ${char.isIndicatable}`}</Text>
      <Text>{`value : ${char.value}`}</Text>
      {/* {<Text>{`descriptors : ${char.descriptors()[0]?.read()}`}</Text>} */}

      <TouchableOpacity>
        <Text>{`isWritableWithResponse : ${char.isWritableWithResponse}`}</Text>
      </TouchableOpacity>
      <Text>{`isWritableWithoutResponse : ${char.isWritableWithoutResponse}`}</Text>
    </TouchableOpacity>
  );
};

const styles = StyleSheet.create({
  container: {
    backgroundColor: 'white',
    marginVertical: 12,
    borderRadius: 16,
    shadowColor: 'rgba(60,64,67,0.3)',
    shadowOpacity: 0.2,
    shadowRadius: 5,
    elevation: 5,
    padding: 12,
  },
  measure: { color: 'red', fontSize: 24 },
  descriptor: { color: 'blue', fontSize: 24 },
});

export { CharacteristicCard };
