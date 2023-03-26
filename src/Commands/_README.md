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
#if COOL_SERIALIZATION
    archive(cereal::make_nvp("Id", command.id),
            cereal::make_nvp("Value", command.value));
#else
        (void)archive;
#endif
}

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetValue<T>& command)
{
#if COOL_SERIALIZATION
    archive(cereal::make_nvp("Id", command.forward_command.id),
            cereal::make_nvp("Value", command.forward_command.value),
            cereal::make_nvp("Old value", command.old_value));
#else
        (void)archive;
#endif
}

} // namespace cereal
```


### If you want it to be stored in the history (ReversibleCommand)

