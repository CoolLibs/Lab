## How to write a new command

- Register it

### Make it serializable

- Make it default-constructible by {}-intializing all the members of the struct
- Define a 
```cpp
namespace ser20 {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetValue<T>& command)
{
    archive(ser20::make_nvp("Id", command.id),
            ser20::make_nvp("Value", command.value));
}

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetValue<T>& command)
{
    archive(ser20::make_nvp("Id", command.forward_command.id),
            ser20::make_nvp("Value", command.forward_command.value),
            ser20::make_nvp("Old value", command.old_value));
}

} // namespace ser20
```


### If you want it to be stored in the history (ReversibleCommand)

