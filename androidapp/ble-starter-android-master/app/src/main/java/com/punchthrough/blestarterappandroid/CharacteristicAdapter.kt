/*
 * Copyright 2019 Punch Through Design LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.punchthrough.blestarterappandroid

import android.bluetooth.BluetoothGattCharacteristic
import android.view.View
import android.view.ViewGroup
//import androidx.recyclerview.widget.RecyclerView
import com.punchthrough.blestarterappandroid.ble.printProperties
//import kotlinx.android.synthetic.main.row_characteristic.view.characteristic_properties
//import kotlinx.android.synthetic.main.row_characteristic.view.characteristic_uuid
import org.jetbrains.anko.layoutInflater

class CharacteristicAdapter(
    private val items: List<BluetoothGattCharacteristic>
//    private val onClickListener: ((characteristic: BluetoothGattCharacteristic) -> Unit)
    )
    //: RecyclerView.Adapter<CharacteristicAdapter.ViewHolder>()
    {

        //access hexadecimal values from "items" which is list of characteristics.
        //this should have each of the 6 channels' values
        // for each of these 6 hex values, convert to number using ascii
        // convert the 6 numbers into a JSON string called instances
        // make API call to pass instances and get back a single 0,1, or 2 value representing ripeness
        // use an enum to map 0/1/2 back to "unripe", "perfectly ripe", or "overripe"
        // Print this value to a string attached to the "result" textview

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = parent.context.layoutInflater.inflate(
            R.layout.row_characteristic,
            parent,
            false
        )
        return ViewHolder(view, onClickListener)
    }

    override fun getItemCount() = items.size
    val instance = listOf<String>()
    fun getInstances() = characteristic.printProperties()

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = items[position]
        holder.bind(item)
    }

    class ViewHolder(
        private val view: View,
        private val onClickListener: ((characteristic: BluetoothGattCharacteristic) -> Unit)
    ) : RecyclerView.ViewHolder(view) {

        fun bind(characteristic: BluetoothGattCharacteristic) {
            view.characteristic_uuid.text = characteristic.uuid.toString()
            view.characteristic_properties.text = characteristic.printProperties()
            view.setOnClickListener { onClickListener.invoke(characteristic) }
        }
    }
}
