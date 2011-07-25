g_GameHeartbeatTime = 
{
1500, --��Ϸ�ٶ�1��
1200,  --2��
1000,  --3��
900,  --4��
800,  --5��
700,  --6��
600,  --7��
500,  --8��
300,  --9��
200   --��Ϸ�ٶ�10��
}
g_GameTollGateSize = 
{
0x000A0010,	--��Ϸ�ؿ�1�������ߴ�(ʮ������ǰ��λ��ʾ������λ��ʾ��)
0x00090011,	--2��
0x00070014,	--3��
0x00050015,	--4��
0x00100010,	--5��
0x0012000E,	--6��
0x0014000C,	--7��
0x00160010,	--8��
0x000F000F,	--9��
0x000A000A,	--��Ϸ�ؿ�10�������ߴ�
0x00190015	--��Ϸ�ؿ�ͨ�س����ߴ�
}

-- ��ȡ�������ָ���
function Sound_GetBackMusicCount( )
    return 0x07;
end

-- ��ȡ���������ļ�·��
function Sound_GetBackMusic( Index )
	if(0 == Index) then
		return "RrTetris\\music\\BackMusic01.mid";
	elseif(1 == Index) then
		return "RrTetris\\music\\BackMusic02.mid";
	elseif(2 == Index) then
		return "RrTetris\\music\\BackMusic03.mid";
	elseif(3 == Index) then
		return "RrTetris\\music\\BackMusic04.mid";
	elseif(4 == Index) then
		return "RrTetris\\music\\BackMusic05.mid";
	elseif(5 == Index) then
		return "RrTetris\\music\\BackMusic06.mid";
	elseif(6 == Index) then
		return "RrTetris\\music\\BackMusic07.mid";
	else
		return "RetError";
	end
end

-- ��ȡ�̶����������ļ�·��
function Sound_GetFixationSound(  )
	return "RrTetris\\music\\�̶�.wav"
end

-- ��ȡ��ը�����ļ�·��
function Sound_GetBlastSound(  )
	return "RrTetris\\music\\��ը.wav"
end

-- ��ȡʧ�������ļ�·��
function Sound_GetDeadSound(  )
	return "RrTetris\\music\\����.wav"
end

-- ��ȡ��ʼ�����ļ�·��
function Sound_GetReadyGoSound(  )
	return "RrTetris\\music\\׼����ʼ.wav"
end

-- ��ȡ��ת���������ļ�·��
function Sound_GetDistortioSound(  )
	return "RrTetris\\music\\��ת.wav"
end

function Sound_Gethurray01Sound(  )
	return "RrTetris\\music\\Combo.ogg"
end

-- ��ȡ����������Ч�ļ�·��
function Sound_Gethurray02Sound(  )
	return "RrTetris\\music\\Great.ogg"
end

-- ��ȡ����������Ч�ļ�·��
function Sound_Gethurray03Sound(  )
	return "RrTetris\\music\\MaxCombo.ogg"
end

-- ��ȡ��Ϸ������Ч�ļ�·��
function Sound_GetLevelUpSound(  )
	return "RrTetris\\music\\LevelUp.ogg"
end

-- ��ȡ��Ϸ���������Ч�ļ�·��
function Sound_GetLevelCompleteSound(  )
	return "RrTetris\\music\\LevelComplete.ogg"
end

-- ��ȡ��ͬ��Ϸ������������
function GetHeartbeatTime( Index )
	if(Index > 10) then
		return "RetError";
	else
		return g_GameHeartbeatTime[Index];
	end
end

-- ��ȡ��ͬ��Ϸ�ؿ�����ĳ�����С
function GetTollGateSize( Index )
	if(Index > g_GameTollGateSize[1]) then
		return "RetError";
	else
		return g_GameTollGateSize[Index];
	end
end

-- ��ȡÿ�������������
function GetEachLevelNeedIntegral(  )
	return 500;
end

-- ��ȡ������IP��ַ��˿�
function GetServerIPAndPort( Index )
	if(1 == Index) then
		return "192.168.111.111";		-- ָ����������IP��ַ
	elseif(2 == Index) then
		return 9999;								-- ָ���������Ķ˿�
	elseif(3 == Index) then
	  return 1;										-- ָ���� "������ = 1" ���� "�ͻ��� = 0"
	else
		return "RetError";
	end
end
