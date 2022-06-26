## How to write a new command

- Register it

### Make it serializable

- Make it default-constructible by {}-intializing all the members of the struct
- Define a 
```cpp
namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetValue<T>& command)
{
    archive(cereal::make_nvp("Id", command.id),
            cereal::make_nvp("Value", command.value));
}

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetValue<T>& command)
{
    archive(cereal::make_nvp("Id", command.forward_command.id),
            cereal::make_nvp("Value", command.forward_command.value),
            cereal::make_nvp("Old value", command.old_value));
}

} // namespace cereal
```


### If you want it to be stored in the history (ReversibleCommand)

