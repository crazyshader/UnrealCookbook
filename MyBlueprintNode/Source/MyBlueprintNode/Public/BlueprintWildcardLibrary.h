// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintWildcardLibrary.generated.h"

#define P_GET_GENERIC_ARRAY(ArrayAddr, ArrayProperty) Stack.MostRecentProperty = nullptr;\
		Stack.StepCompiledIn<FArrayProperty>(NULL);\
		void* ArrayAddr = Stack.MostRecentPropertyAddress;\
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);\
		if (!ArrayProperty) {	Stack.bArrayContextFailed = true;	return; }

USTRUCT(BlueprintInternalUseOnly)
struct FDummyStruct {
	GENERATED_USTRUCT_BODY()

};

/**
 *
 */
UCLASS()
class MYBLUEPRINTNODE_API UBlueprintWildcardLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "MyDemo", meta = (CustomStructureParam = "CustomStruct"))
		static void ShowStructFields(const FDummyStruct& CustomStruct);
	static void Generic_ShowStructFields(const void* StructAddr, const FStructProperty* StructProperty);


	UFUNCTION(BlueprintPure, CustomThunk, meta = (DisplayName = "Array Numeric Property Average", ArrayParm = "TargetArray", ArrayTypeDependentParams = "TargetArray"), Category = "MyDemo")
		static float Array_NumericPropertyAverage(const TArray<int32>& TargetArray, FName PropertyName);
	static float GenericArray_NumericPropertyAverage(const void* TargetArray, const FArrayProperty* ArrayProperty, FName ArrayPropertyName);

public:
	DECLARE_FUNCTION(execShowStructFields) {

		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentProperty = nullptr;

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);


		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_ShowStructFields(StructAddr, StructProperty);
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execArray_NumericPropertyAverage) {

		// get TargetArray
		P_GET_GENERIC_ARRAY(ArrayAddr, ArrayProperty);
		//P_GET_TARRAY_REF(int32, TargetArray);

		// get PropertyName
		P_GET_PROPERTY(FNameProperty, PropertyName);

		P_FINISH;

		P_NATIVE_BEGIN;
		*(float*)RESULT_PARAM = GenericArray_NumericPropertyAverage(ArrayAddr, ArrayProperty, PropertyName);
		P_NATIVE_END;
	}
};
