/*
 * TypeDenoter.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_TYPE_DENOTER_H
#define XSC_TYPE_DENOTER_H


#include "Visitor.h"
#include "ASTEnums.h"
#include <memory>
#include <string>


namespace Xsc
{


// Type denoter base class.
struct TypeDenoter
{
    enum class Types
    {
        Void,
        Base,
        Buffer,
        Texture,
        Sampler,
        Struct,
        Alias,
        Array,
    };

    virtual ~TypeDenoter();

    // Returns the type (kind) of this type denoter.
    virtual Types Type() const = 0;

    // Returns a simple string representation of this type denoter (e.g. "scalar type").
    virtual std::string ToString() const = 0;

    // Returns either this or the aliased type.
    virtual TypeDenoter* Get();

    virtual bool IsScalar() const;
    virtual bool IsVector() const;
    virtual bool IsMatrix() const;

    bool IsVoid() const;
    bool IsBuffer() const;
    bool IsSampler() const;
    bool IsTexture() const;
    bool IsStruct() const;
    bool IsArray() const;

    // Returns true if this type denoter is compatible with the specified type denoter (special cases for void and base types).
    virtual bool IsCompatibleWith(const TypeDenoter& rhs) const;

    // Returns true if this type denoter can be casted to the specified target type denoter (special cases void and base types).
    virtual bool IsCastableTo(const TypeDenoter& targetType) const;
};

using TypeDenoterPtr = std::shared_ptr<TypeDenoter>;

// Void type denoter.
struct VoidTypeDenoter : public TypeDenoter
{
    Types Type() const override;
    std::string ToString() const override;

    // Returns always false, since void type can not be casted to anything.
    bool IsCastableTo(const TypeDenoter& targetType) const override;
};

using VoidTypeDenoterPtr = std::shared_ptr<VoidTypeDenoter>;

// Base type denoter.
struct BaseTypeDenoter : public TypeDenoter
{
    Types Type() const override;
    std::string ToString() const override;

    bool IsScalar() const override;
    bool IsVector() const override;
    bool IsMatrix() const override;

    bool IsCastableTo(const TypeDenoter& targetType) const;

    DataType dataType = DataType::Undefined;
};

using BaseTypeDenoterPtr = std::shared_ptr<BaseTypeDenoter>;

// Buffer type denoter.
struct BufferTypeDenoter : public TypeDenoter
{
    Types Type() const override;
    std::string ToString() const override;

    BufferDeclStmnt* bufferDeclRef = nullptr;
};

using BufferTypeDenoterPtr = std::shared_ptr<BufferTypeDenoter>;

// Texture type denoter.
struct TextureTypeDenoter : public TypeDenoter
{
    Types Type() const override;
    std::string ToString() const override;

    TextureDecl* textureDeclRef = nullptr;
};

using TextureTypeDenoterPtr = std::shared_ptr<TextureTypeDenoter>;

// Sampler type denoter.
struct SamplerTypeDenoter : public TypeDenoter
{
    Types Type() const override;
    std::string ToString() const override;

    SamplerDecl* samplerDeclRef = nullptr;
};

using SamplerTypeDenoterPtr = std::shared_ptr<SamplerTypeDenoter>;

// Struct type denoter.
struct StructTypeDenoter : public TypeDenoter
{
    StructTypeDenoter() = default;
    StructTypeDenoter(const std::string& ident);
    StructTypeDenoter(Structure* structDeclRef);

    Types Type() const override;
    std::string ToString() const override;

    std::string     ident;
    Structure*      structDeclRef = nullptr;
};

using StructTypeDenoterPtr = std::shared_ptr<StructTypeDenoter>;

// Alias type denoter.
struct AliasTypeDenoter : public TypeDenoter
{
    Types Type() const override;
    std::string ToString() const override;

    TypeDenoter* Get() override;

    std::string     ident;                  // Type identifier
  //StructurePtr    structDecl;             // Optional struct-decl AST node.
    TypeDenoter*    aliasTypeRef = nullptr;
};

using AliasTypeDenoterPtr = std::shared_ptr<AliasTypeDenoter>;

// Array type denoter.
struct ArrayTypeDenoter : public TypeDenoter
{
    Types Type() const override;
    std::string ToString() const override;

    TypeDenoterPtr          baseTypeDenoter;
    std::vector<ExprPtr>    arrayDims;
};

using ArrayTypeDenoterPtr = std::shared_ptr<ArrayTypeDenoter>;



} // /namespace Xsc


#endif



// ================================================================================