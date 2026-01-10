#pragma once

#include <winrt/base.h>

namespace winrt::impl {
    template <typename Async>
    auto wait_for(Async const& async, Windows::Foundation::TimeSpan const& timeout);
}

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Radios.h>
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include <winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Storage.Streams.h>


template <typename T>
using WinIAsyncOperation = winrt::Windows::Foundation::IAsyncOperation<T>;
using WinAsyncStatus = winrt::Windows::Foundation::AsyncStatus;

using WinBluetoothAdapter = winrt::Windows::Devices::Bluetooth::BluetoothAdapter;
using WinDeviceInformation = winrt::Windows::Devices::Enumeration::DeviceInformation;
using WinDeviceInformationUpdate = winrt::Windows::Devices::Enumeration::DeviceInformationUpdate;
using WinDeviceWatcher = winrt::Windows::Devices::Enumeration::DeviceWatcher;


using WinAdvWatcher = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
using WinAdvArgs = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
using WinAdvFilter = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementFilter;
using WinAdvertisement = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisement;
using WinAdvDataSection = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementDataSection;
using WinAdvManufacturerData = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEManufacturerData;

using WinScanMode = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode;
using WinNativeDevice = winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;
using WinConnectionStatus = winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus;

using WinGattSession = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattSession;
using WinGattDeviceServicesResult = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceServicesResult;
using WinGattDeviceService = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService;
using WinGattCharacteristic = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic;
using WinGattValueChangedEventArgs = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs;
using WinGattCharacteristicsResult = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicsResult;

using WinDeviceParingResult = winrt::Windows::Devices::Enumeration::DevicePairingResult;
using WinDevicePairingResultStatus = winrt::Windows::Devices::Enumeration::DevicePairingResultStatus;
using WinDeviceUnpairingResultStatus = winrt::Windows::Devices::Enumeration::DeviceUnpairingResultStatus;

using WinDevicePairingProtectionLevel = winrt::Windows::Devices::Enumeration::DevicePairingProtectionLevel;
using WinDevicePairingKinds = winrt::Windows::Devices::Enumeration::DevicePairingKinds;

using WinGattReadResult = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattReadResult;
using WinGattCommunicationStatus = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus;
using WinGattWriteResult = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattWriteResult;
using WinIBuffer = winrt::Windows::Storage::Streams::IBuffer;
using WinBuffer = winrt::Windows::Storage::Streams::Buffer;
using WinDataWriter = winrt::Windows::Storage::Streams::DataWriter;
using WinDataReader = winrt::Windows::Storage::Streams::DataReader;

using WinGattClientCharConfDescVal = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattClientCharacteristicConfigurationDescriptorValue;

template<typename T>
using WinIVectorView = winrt::Windows::Foundation::Collections::IVectorView<T>;
using WinGuidHelper = winrt::Windows::Foundation::GuidHelper;

using WinRTRadio = winrt::Windows::Devices::Radios::Radio;
using WinRTRadioKind = winrt::Windows::Devices::Radios::RadioKind;
using WinRTRadioState = winrt::Windows::Devices::Radios::RadioState;
using WinRTInspectable = winrt::Windows::Foundation::IInspectable;

using WinRTLowEnergyAdvDataWatcher = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
using WinRTLowEnergyScaningMode = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode;
using WinRTLowEnergyAdvData = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
