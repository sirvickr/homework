object FDB: TFDB
  OldCreateOrder = False
  Left = 551
  Top = 139
  Height = 433
  Width = 249
  object radusers: TADOConnection
    CommandTimeout = 60
    Connected = True
    ConnectionString = 
      'Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security In' +
      'fo=False;Initial Catalog=radusers;Data Source=COUNTER'
    ConnectionTimeout = 30
    LoginPrompt = False
    Mode = cmReadWrite
    Provider = 'SQLOLEDB.1'
    Left = 8
    Top = 4
  end
  object Q_user_id_list: TADODataSet
    Connection = radusers
    CommandText = 
      'SELECT DISTINCT active_user_id'#13#10'FROM users_for_sale '#13#10'ORDER BY a' +
      'ctive_user_id'
    Parameters = <>
    Left = 80
    Top = 4
  end
  object Proc_Read_User_Info2: TADOStoredProc
    Connection = radusers
    ProcedureName = 'Proc_Read_User_Info2'
    Parameters = <
      item
        Name = '@RETURN_VALUE'
        DataType = ftInteger
        Direction = pdReturnValue
        Precision = 10
      end
      item
        Name = '@selectID'
        Attributes = [paNullable]
        DataType = ftString
        Size = 50
      end>
    Left = 80
    Top = 56
  end
  object FSeries_CboProfile: TADODataSet
    Active = True
    Connection = radusers
    CursorType = ctStatic
    CommandText = 
      'SELECT profile_cod, profile_description, profile_pref, profile_t' +
      'ime '#13#10'FROM PROFILE_COD'#13#10'ORDER BY profile_order'
    Parameters = <>
    Left = 84
    Top = 116
  end
  object Proc_Activate_SerieNew: TADOStoredProc
    Connection = radusers
    ProcedureName = 'Proc_Activate_SerieNew'
    Parameters = <
      item
        Name = '@RETURN_VALUE'
        DataType = ftInteger
        Direction = pdReturnValue
        Precision = 10
      end
      item
        Name = '@ActivProfile'
        Attributes = [paNullable]
        DataType = ftString
        Size = 50
      end
      item
        Name = '@N_ID'
        Attributes = [paNullable]
        DataType = ftInteger
        Precision = 10
      end
      item
        Name = '@MinID'
        Attributes = [paNullable]
        DataType = ftString
        Direction = pdInputOutput
        Size = 50
      end
      item
        Name = '@MaxID'
        Attributes = [paNullable]
        DataType = ftString
        Direction = pdInputOutput
        Size = 50
      end>
    Left = 84
    Top = 176
  end
  object P_GetCardsWithProfileInfo: TADOStoredProc
    Connection = radusers
    ProcedureName = 'P_GetCardsWithProfileInfo'
    Parameters = <
      item
        Name = '@RETURN_VALUE'
        DataType = ftInteger
        Direction = pdReturnValue
        Precision = 10
      end
      item
        Name = '@IDPrintmin'
        Attributes = [paNullable]
        DataType = ftString
        Size = 50
      end
      item
        Name = '@IDPrintmax'
        Attributes = [paNullable]
        DataType = ftString
        Size = 50
      end>
    Left = 84
    Top = 236
  end
end
