
//type string enum in "A" "B" "C" "D"

//data string like "552549 35017.8 1503.21 3068.94 1957.15 276.708 1824.81 4374.44 7493.05 1834.76 0 135.831 45.2355 0 53463.9 4320.57 120.937 84.1033 27.9854 2580.32 330.484 258.054 212.745 211.342 1563.93 2239.54 1045.48 894.954 1572.68 605.845 391.512 170.966 67.1708 3077 286.395 2081.39 202.392 1459.84 0 444.34 248.397 215.958 126.06 157.738 514.565 394.031 84.6821 140.302 663.384 10673 230 250 315.579 946.613 101.548 561.535 209.986 27.0882 195.133 736.945 1110.64 1613.11 701.754 37119.4 13918.3 37.6361 8763.94 6330.21 6.62152 7309.61 49622.8 4 340.403 93092 3.5657 1659.21 9251.85 2491.19 0.01 351.141 1441.92 834.115 335.854 496.285 274.853 3.00226e+06 2.36285e+06 11247 2 324.391 12761.5 1.28141e+06 524.106 75.1258 60.61 6698.35 197.928 72.781 4.85202 301.288 101.87 1226.07"

//data column name is : Income_AnnualIncome	Income_MonthlyIncome	Income_Pension	Income_Bonus	Income_TransportationSubsidies	Income_Tips	Income_Commission	Income_TaxExemption	Income_RentalIncome	Income_Dividends	Income_SocialSecurityAllowance	Income_OldAgeAllowance	Income_DisabilityAllowance	Income_OtherSocialAllowance	Income_MonthlyTotal	Expense_AlimonyChildSupportPayments	Expense_AutoGasOil	Expense_AutoInsurance	Expense_AutoRepair	Expense_AutoParkingTolls	Expense_Books	Expense_Periodicals	Expense_Subscriptions	Expense_CharitableGiving	Expense_ChildExpensesTuitionSchoolFees	Expense_ChildExpensesExtracurricularActivities	Expense_ChildExpensesChildCare	Expense_Clothing	Expense_DiningOut	Expense_EntertainmentRecreation	Expense_Hobbies	Expense_Gifts	Expense_Groceries	Expense_MaintanenceFees	Expense_HomeownerInsurance	Expense_Homemaid 	Expense_HomeMaintenanceRepair	Expense_HouseKeeping	Expense_LandscapingYard	Expense_MedicalExpenseCopaysDeductibles	Expense_MedicalExpenseInsurancePremium	Expense_MedicalExpensePrescriptions	Expense_MembershipDuesRecreation	Expense_MembershipDuesProfessionalAssociations	Expense_Education	Expense_Miscellaneous	Expense_PersonalCareHair	Expense_PersonalCareDryCleaning	Expense_PetExpenses	Expense_RentLeasePayment	Expense_UtilitiesCableTV	Expense_UtilitiesInternet	Expense_UtilitiesGas	Expense_UtilitiesElectricity	Expense_UtilitiesTelephone	Expense_UtilitiesCellPhone	Expense_UtilitiesWater	Expense_UtilitiesSewer	Expense_UtilitiesTrashRemoval	Expense_VacationPersonalTravel	Expense_IncomeTax	Expense_MPF	Expense_HouseImprovements	Expense_MonthlyTotal	Liability_CreditCards_TotalBalanceDue	Liability_CreditCards_InterestPercentage	Liability_CreditCards_MonthlyPayment	Liability_LineOfCredits_TotalBalanceDue	Liability_LineOfCredits_InterestPercentage	Liability_LineOfCredits_MonthlyPayment	Liability_StudentLoan_TotalBalanceDue	Liability_StudentLoan_InterestPercentage	Liability_StudentLoan_MonthlyPayment	Liability_AutoLoan_TotalBalanceDue	Liability_AutoLoan_InterestPercentage	Liability_AutoLoan_MonthlyPayment	Liability_MonthlyTotal	Investment_Savings	Investment_SavingInterestPercentage	Investment_ForeignCurrency	Investment_Stock	Investment_LifeInsurance	Investment_Bonds	Investment_MoneyMarketFunds	Investment_Gamble	Investment_FairMarketValue	Investment_MortgageBalance	Investment_MonthlyPayment	Investment_Interest	Investment_ProprietyTax	Investment_MonthlyTotal	Insurance_LifeInsurance_Amount	Insurance_LifeInsurance_Premium	Insurance_Disability_WaitingPeriodDays	Insurance_Disability_BenefitAge	Insurance_Disability_BenefitAmount	Insurance_Disability_Premium	Insurance_LongTermCare_WaitingPeriodDays	Insurance_LongTermCare_BenefitPeriodYears	Insurance_LongTermCare_BenefitAmount 	Insurance_LongTermCare_Premium	Insurance_MonthlyTotal

service SimilarityJoinService
{

	//use id to query mate Data
	list<string> QueryDataByID        (1: list<i32> UserIDs);

	//use id to query Type 
	list<string> QueryTypeByID        (1: list<i32> UserIDs);

	//use id to query index Type distributed
	list<i32>    QueryDistributedByID (1: list<i32> UserIDs);

	//use data to query Type 
	list<string> QueryTypeByData      (1: list<string> Datas);

	//return each type count in the index.
	list<i32>    GetIndexDistributed  ();

	//ReslSize Capacity MemorySize SystemMemory
	list<string> GetIndexMetaData     ();

	//SelfQuery LSH Token GetData FilterK TotalQuery
	list<string> GetLatestQueryTime     ();

	//use data and return id
	list<i32>    JoinByStrategy1      (1: list<string> Datas, 2: i32 ThresholdK, 3: i32 TimeOut);

	//use data and return id
	list<i32>    JoinByStrategy2      (1: list<string> Datas, 2: i32 ThresholdK, 3: i32 TimeOut);

	//use data and return id
	list<i32>    JoinByStrategy3      (1: list<string> Datas, 2: i32 ThresholdK, 3: i32 TimeOut, 4: double SelfQueryR);

}