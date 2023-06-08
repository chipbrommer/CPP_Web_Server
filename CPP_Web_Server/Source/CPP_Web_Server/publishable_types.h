///////////////////////////////////////////////////////////////////////////////
//!
//! @file       publishable_types.h
//! 
//! @brief      Definitions for types publishable to the webserver class. 
//! 
//! @author     Chip Brommer
//!
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//    Defines:
//          name                        reason defined
//          --------------------        ---------------------------------------
#ifndef     CPP_PUBLISHABLE_TYPES       // Define the header file
#define     CPP_PUBLISHABLE_TYPES
//
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
    namespace Communications
    {
        typedef void (*Funcptr)();

        namespace Data
        {
            enum class Type
            {
                NONE,
                CHAR,
                UCHAR,
                SHORT,
                USHORT,
                INT,
                UINT,
                DOUBLE,
                FLOAT,
                STRING,
                BOOL,
            };

            static std::map<Type, std::string> TypeMap
            {
                {Type::NONE,    std::string("none")},
                {Type::CHAR,    std::string("char")},
                {Type::UCHAR,   std::string("unsigned char")},
                {Type::SHORT,   std::string("short")},
                {Type::USHORT,  std::string("unsigned short")},
                {Type::INT,     std::string("int")},
                {Type::UINT,    std::string("unsigned int")},
                {Type::DOUBLE,  std::string("double")},
                {Type::FLOAT,   std::string("float")},
                {Type::STRING,  std::string("string")},
                {Type::BOOL,    std::string("bool")},
            };

            enum class Access
            {
                HIDDEN,
                VIEW,
                EDIT,
                VIEW_EDIT,
            };
        };

        namespace Function
        {
            enum class Access
            {
                HIDDEN,
                EXECUTE,
                EXECUTE_READBACK,
            };
        }

        namespace Graph
        {
            enum class Type
            {
                NONE,
                LINE,
                BAR,
            };

            static std::map<Type, std::string> TypeMap
            {
                {Type::NONE,   std::string("none")},
                {Type::LINE,   std::string("line")},
                {Type::BAR,    std::string("bar")},
            };
        }

#pragma pack(push,1)
        struct PublishedGraphData
        {
            void*           address;
            std::string     unique_name;
            std::string     description;
            Data::Type      type;
            Data::Access    access;
            std::string     graph_name;
            Graph::Type     graph_type;
            int             graph_size;

            PublishedGraphData()
            {
                address     = nullptr;
                unique_name = "";
                description = "";
                type        = Data::Type::NONE;
                access      = Data::Access::VIEW;
                graph_name  = "";
                graph_type  = Graph::Type::NONE;
                graph_size  = 0;
            }

            PublishedGraphData(void* new_address, std::string name, std::string new_description, Data::Type new_type, std::string new_graph_name, Graph::Type new_graph_type, int max_graph_size)
            {
                address     = new_address;
                unique_name = name;
                description = new_description;
                type        = new_type;
                access      = Data::Access::VIEW;
                graph_name  = new_graph_name;
                graph_type  = new_graph_type;
                graph_size  = max_graph_size;
            }

            PublishedGraphData(std::string name)
            {
                PublishedGraphData();
                this->unique_name = name;
            }

            std::string Peek()
            {
                //this is only if for if it's a string
                //It can't be done in a switch/case for some reason...
                std::string* tmp_string = static_cast<std::string*>(address);

                std::string ret;
                switch (type)
                {
                case Data::Type::NONE:
                    break;
                case Data::Type::CHAR:
                    ret = std::to_string(*(char*)address);
                    break;
                case Data::Type::UCHAR:
                    ret = std::to_string(*(unsigned char*)address);
                    break;
                case Data::Type::SHORT:
                    ret = std::to_string(*(short*)address);
                    break;
                case Data::Type::USHORT:
                    ret = std::to_string(*(unsigned short*)address);
                    break;
                case Data::Type::INT:
                    ret = std::to_string(*(int*)address);
                    break;
                case Data::Type::UINT:
                    ret = std::to_string(*(unsigned int*)address);
                    break;
                case Data::Type::DOUBLE:
                    ret = std::to_string(*(double*)address);
                    break;
                case Data::Type::BOOL:
                    if (true == *(bool*)address)
                    {
                        ret = "true";
                    }
                    else
                    {
                        ret = "false";
                    }
                    break;
                case Data::Type::FLOAT:
                    ret = std::to_string(*(float*)address);
                    break;
                case Data::Type::STRING:
                    ret = *tmp_string;
                    break;
                default:
                    ret = "Unsupported Type";
                }
                return ret;
            }
        };
#pragma pack(pop)

#pragma pack(push,1)
        struct PublishedData
        {
            void*               address;
            std::string         unique_name;
            std::string         description;
            Data::Type          type;
            Data::Access        access;

            PublishedData()
            {
                address       = nullptr;
                unique_name   = "";
                description   = "";
                type          = Data::Type::NONE;
                access        = Data::Access::VIEW;
            }

            PublishedData(void* new_address, std::string name, std::string new_description, Data::Type new_type)
            {
                this->address       = new_address;
                this->unique_name   = name;
                this->description   = new_description;
                this->type          = new_type;
                this->access        = Data::Access::VIEW;
            }

            PublishedData(std::string name)
            {
                PublishedData();
                this->unique_name   = name;
            }

            std::string Peek()
            {
                //this is only if for if it's a string
                //It can't be done in a switch/case for some reason...
                std::string* tmp_string = static_cast<std::string*>(address);

                std::string ret;
                switch (type)
                {
                case Data::Type::NONE:
                    break;
                case Data::Type::CHAR:
                    ret = std::to_string(*(char*)address);
                    break;
                case Data::Type::UCHAR:
                    ret = std::to_string(*(unsigned char*)address);
                    break;
                case Data::Type::SHORT:
                    ret = std::to_string(*(short*)address);
                    break;
                case Data::Type::USHORT:
                    ret = std::to_string(*(unsigned short*)address);
                    break;
                case Data::Type::INT:
                    ret = std::to_string(*(int*)address);
                    break;
                case Data::Type::UINT:
                    ret = std::to_string(*(unsigned int*)address);
                    break;
                case Data::Type::DOUBLE:
                    ret = std::to_string(*(double*)address);
                    break;
                case Data::Type::BOOL:
                    if (true == *(bool*)address)
                    {
                        ret = "true";
                    }
                    else
                    {
                        ret = "false";
                    }
                    break;
                case Data::Type::FLOAT:
                    ret = std::to_string(*(float*)address);
                    break;
                case Data::Type::STRING:
                    ret = *tmp_string;
                    break;
                default:
                    ret = "Unsupported Type";
                }
                return ret;
            }
        };
#pragma pack(pop)

#pragma pack(push,1)
        struct PublishedFunction
        {
            Funcptr                 address;
            //ObjectPointer<C> owner;
            std::string             unique_name;
            std::string             description;
            Data::Type              return_type;
            Function::Access        access;
            std::vector<Data::Type> args;

            PublishedFunction()
            {
                address     = nullptr;
                // owner    = nullptr;
                unique_name = "";
                description = "";
                return_type = Data::Type::NONE;
                access      = Function::Access::EXECUTE;
            }

            PublishedFunction(std::string name)
            {
                PublishedFunction();
                this->unique_name = name;
            }
            PublishedFunction(Funcptr new_address, std::string name, std::string new_description)
            {
                address = new_address;
                //  owner = nullptr;
                unique_name = name;
                description = new_description;
                return_type = Data::Type::NONE;
                access = Function::Access::EXECUTE;
            }

            // PublishedFunction(void* object, Funcptr new_address, std::string name, std::string new_description)
            // {   
            //     address = new_address;
            //     owner.set_object(object);
            //     unique_name = name;
            //     description = new_description;
            //     return_type = Data::Type::NONE;
            //     access = Function::Access::EXECUTE;
            // }

            PublishedFunction operator=(const PublishedFunction& old)
            {
                PublishedFunction ret("ret");
                ret.address     = old.address;
                ret.unique_name = old.unique_name;
                ret.description = old.description;
                ret.return_type = old.return_type;
                ret.access      = old.access;
                return ret;
            }
            void Execute()
            {
                Funcptr function = address;
                // if(nullptr == owner.get_object())
                // {
                function();
                //}
                // else
                // {
                //     owner.get_object()->f();
                // }
            }
        };
#pragma pack(pop)

    } // End Communications
} // End Essentials

#endif // CPP_PUBLISHABLE_TYPES