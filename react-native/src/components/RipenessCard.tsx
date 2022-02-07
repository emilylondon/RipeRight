import React, { useEffect, useState } from 'react';
import { StyleSheet, Text, TouchableOpacity, View } from 'react-native';
import { Characteristic, Descriptor, Service } from 'react-native-ble-plx';
import { CharacteristicCard } from './CharacteristicCard';
import { DescriptorCard } from './DescriptorCard';
import { Base64 } from '../lib/base64'
import { ResultCard } from './ResultCard'

type RipenessCardProps = {
  service: Service;
};

const decodeBleString = (value: string | undefined | null): string => {
    if (!value) {
      return '';
    }
    return Base64.decode(value).charCodeAt(0);
  };

// eslint-disable-next-line @typescript-eslint/no-unused-vars
const UART_SERVICE_UUID = '6E400001-B5A3-F393-­E0A9-­E50E24DCCA9E'.toLowerCase();

const RipenessCard = ({ service }: RipenessCardProps) => {
  const [descriptors, setDescriptors] = useState<Descriptor[]>([]);
  const [characteristics, setCharacteristics] = useState<Characteristic[]>([]);
  const [isRipenessVisible, setIsRipenessVisible] = useState(false);
  const [measurements, setMeasurements] = useState<String[]>([]);
  const [ripeness, setRipeness] = useState(null);

  useEffect(() => {
    const getCharacteristics = async () => {
      const newCharacteristics = await service.characteristics();
      setCharacteristics(newCharacteristics);
      var ripenessMeansurementIds: Array<String> = [
          'sensorR',
          'sensorS',
          'sensorT',
          'sensorU',
          'sensorV',
          'sensorW'
      ]
      newCharacteristics.forEach(async (characteristic) => {
        const newDescriptors = await characteristic.descriptors();
        setDescriptors((prev) => [...new Set([...prev, ...newDescriptors])]);
        characteristic.descriptors().then((desc) => {
            desc[0]?.read().then((val) => {
                if (val) {
                    if (ripenessMeansurementIds.indexOf(Base64.decode(val.value)) > -1){
                        setMeasurements((prev) => [...new Set([...prev, ...decodeBleString(characteristic?.value)])]);
                    }
                }
            });
        });
    });

    const makeApiCall = async () => {

    }

    };

    getCharacteristics();
    
  }, [service]);

  return (
    <View style={styles.container}>

      {isRipenessVisible && 
        <ResultCard ripeness = {ripeness} />}
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    backgroundColor: 'white',
    marginBottom: 12,
    borderRadius: 16,
    shadowColor: 'rgba(60,64,67,0.3)',
    shadowOpacity: 0.4,
    shadowRadius: 10,
    elevation: 4,
    padding: 12,
  },
});

export { RipenessCard };
