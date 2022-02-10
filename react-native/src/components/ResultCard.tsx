import React, { useEffect, useState } from 'react';
import { StyleSheet, Text, TouchableOpacity, View } from 'react-native';

const ResultCard = (props : any) => {
    return (
    <View>
        <Text>Ripeness result</Text>
        <Text>The fruit is { props.ripeness ? 2 : "overripe" ? 1 : "perfectly ripe" ? 0 : "underripe" }
        </Text>
    </View>
    );
}

export { ResultCard };