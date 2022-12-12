void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

class RallyCP
{
	string Name;
	string CPType;
	vector Position;
	float Radius;
	
	void RallyCP(string name, string cpType, vector position, float radius)
	{
		this.Name = name;
		this.CPType = cpType;
		this.Position = position;
		this.Radius = radius;
	}
}

class RallyCompetitorData
{
	string GUID;
	string Name;
	bool Started;
	bool Finished;
	float StartTime;
	float FinishTime;
	autoptr TStringArray CPs = new TStringArray; 
	
	void RallyCompetitorData(string gUID, string name)
	{
		this.GUID = gUID;
		this.Name = name;
	}
}

vector RallyResetPos = Vector(1645.750000, 451.746002, 14190.099609);
float RallyResetRadius = 100;
float RallyTimer;
float RallyResultsTimer;
float RallyPenalty;
ref array<ref RallyCP> RallyCPs = new array<ref RallyCP>();
ref array<ref RallyCompetitorData> RallyCompetitors = new array<ref RallyCompetitorData>();
autoptr TStringArray RallyAdmins = new TStringArray; 

class CustomMission: MissionServer
{
	void CustomMission()
    {
		RallyTimer = 0;
		RallyResultsTimer = 0;
		RallyPenalty = 30;
		RallyCPs.Insert(new RallyCP("Start", "S", Vector(547.258972, 421.897003, 11117.799805), 5));
		RallyCPs.Insert(new RallyCP("CP1", "CP", Vector(1120.680054, 331.583008, 11326.200195), 10));
		RallyCPs.Insert(new RallyCP("CP2", "CP", Vector(1515.540039, 290.332001, 11134.900391), 10));
		RallyCPs.Insert(new RallyCP("CP3", "CP", Vector(1838.489990, 259.540985, 11476.200195), 10));
		RallyCPs.Insert(new RallyCP("CP4", "CP", Vector(2261.889893, 258.274994, 9352.429688), 10));
		RallyCPs.Insert(new RallyCP("CP5", "CP", Vector(1921.910034, 265.834015, 7492.140137), 10));
		RallyCPs.Insert(new RallyCP("CP6", "CP", Vector(2932.510010, 333.817993, 7378.100098), 10));
		RallyCPs.Insert(new RallyCP("CP7", "CP", Vector(2937.250000, 324.523010, 7721.250000), 10));
		RallyCPs.Insert(new RallyCP("CP8", "CP", Vector(4183.009766, 290.013000, 7264.100098), 10));
		RallyCPs.Insert(new RallyCP("CP9", "CP", Vector(4562.819824, 280.816010, 6864.509766), 10));
		RallyCPs.Insert(new RallyCP("CP10", "CP", Vector(4836.569824, 268.071014, 6787.470215), 10));
		RallyCPs.Insert(new RallyCP("CP11", "CP", Vector(5683.680176, 303.656006, 7258.209961), 10));
		RallyCPs.Insert(new RallyCP("CP12", "CP", Vector(7144.270020, 294.018005, 7696.180176), 10));
		RallyCPs.Insert(new RallyCP("CP13", "CP", Vector(7298.620117, 234.604004, 6576.419922), 10));
		RallyCPs.Insert(new RallyCP("CP14", "CP", Vector(7545.209961, 213.970001, 5148.220215), 10));
		RallyCPs.Insert(new RallyCP("CP15", "CP", Vector(8981.500000, 116.407997, 4941.939941), 10));
		RallyCPs.Insert(new RallyCP("CP16", "CP", Vector(10328.299805, 50.548000, 3824.800049), 10));
		RallyCPs.Insert(new RallyCP("CP17", "CP", Vector(10187.700195, 50.520599, 3823.300049), 10));
		RallyCPs.Insert(new RallyCP("CP18", "CP", Vector(10124.500000, 74.531502, 3733.719971), 10));
		RallyCPs.Insert(new RallyCP("CP19", "CP", Vector(10456.400391, 6.039720, 2515.979980), 10));
		RallyCPs.Insert(new RallyCP("Finish", "F", Vector(10190.599609, 24.241400, 2543.530029), 10));
		RallyAdmins.Insert("7xxxxxxxxxxxxxxx9");
		RallyAdmins.Insert("7xxxxxxxxxxxxxxx2");
    }
	
	RallyCompetitorData GetCompetitor(string gUID, string name)
	{
		foreach (RallyCompetitorData competitor : RallyCompetitors)
		{
			if(competitor.GUID == gUID) return competitor;
		}
		
		RallyCompetitorData newCompetitor = new RallyCompetitorData(gUID, name);
		RallyCompetitors.Insert(newCompetitor);
		return newCompetitor;
	}
	
	bool RallyCheckCP(string GUID, string CPName)
	{
		foreach (RallyCompetitorData cpData : RallyCompetitors)
		{
			if(cpData.GUID == GUID && cpData.CPs.Find(CPName) >= 0) return true;
		}
		
		return false;
	}
	
	void RallySetCP(string GUID, string CPName)
	{
		foreach (RallyCompetitorData cpData : RallyCompetitors)
		{
			if(cpData.GUID == GUID)
			{
				cpData.CPs.RemoveItem(CPName);
				cpData.CPs.Insert(CPName);
				return;
			}
		}
	}
	
	int RallyGetPenlaty(string GUID)
	{
		int result = 0;
		
		foreach (RallyCompetitorData cpData : RallyCompetitors)
		{
			if(cpData.GUID == GUID)
			{
				foreach (RallyCP cp : RallyCPs)
				{
					if (!RallyCheckCP(GUID, cp.Name))
					{
						result++;
					}
				}
			}
		}
		
		return result;
	}
	
	string RallyGetCPString(string GUID)
	{
		string result = "";
		
		foreach (RallyCompetitorData cpData : RallyCompetitors)
		{
			if(cpData.GUID == GUID)
			{
				foreach (RallyCP cp : RallyCPs)
				{
					if (RallyCheckCP(GUID, cp.Name))
					{
						result += "+ ";
					}
					else
					{
						result += "- ";
					}
				}
				
				return result;
			}
		}
		
		return result;
	}
	
	string RallyGetTimeString(float timeSeconds)
	{
		int totalMs = timeSeconds * 1000; 
		int hours = totalMs / 3600000;
		int mins = (totalMs % 3600000) / 60000;
		int seconds = totalMs % 60000 / 1000;
		int millisecs = totalMs % 1000;
		string h;
		string m;
		string s;
		string mm;
		
		if (hours < 10)
		{
			h = string.Format("0%1", hours);
		}
		else
		{
			h = string.Format("%1", hours);
		}
		
		if (mins < 10)
		{
			m = string.Format("0%1", mins);
		}
		else
		{
			m = string.Format("%1", mins);
		}
		
		if (seconds < 10)
		{
			s = string.Format("0%1", seconds);
		}
		else
		{
			s = string.Format("%1", seconds);
		}
		
		if (millisecs < 10)
		{
			mm = string.Format("00%1", millisecs);
		}
		else if (millisecs < 100)
		{
			mm = string.Format("0%1", millisecs);
		}
		else
		{
			mm = string.Format("%1", millisecs);
		}
		
		string result = string.Format("%1:%2:%3.%4", h, m, s, mm);
		return result;
	}
	
	void RallyAdminMSG(string msg)
	{
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);
		
		foreach (Man player : players)
		{
			string GUID = player.GetIdentity().GetPlainId();
			
			if (RallyAdmins.Find(GUID) >= 0)
			{
				PlayerBase pb = PlayerBase.Cast(player);
				pb.MessageImportant(msg);
			}
		}
	}
	
	void RallyResultsDump()
	{
		string msg;
		int penalties;
		float finalTime;
		Print("***************************** ");
		Print("**** Rally Results Start **** ");
		
		foreach (RallyCompetitorData cpData : RallyCompetitors)
		{
			if (cpData.Finished)
			{
				penalties = RallyGetPenlaty(cpData.GUID);
				finalTime = cpData.FinishTime - cpData.StartTime;
				msg = cpData.Name + " finished Time: " + RallyGetTimeString(finalTime) + " Final Time: " + RallyGetTimeString(finalTime + (penalties * RallyPenalty)) + " Penalties x " + penalties +  " [ " + RallyGetCPString(cpData.GUID) + "]";
				Print("**** " + msg);
			}
			else if (cpData.Started)
			{
				penalties = RallyGetPenlaty(cpData.GUID);
				msg = cpData.Name + " started [ " + RallyGetCPString(cpData.GUID) + "]";
				Print("**** " + msg);
			}
		}
		
		Print("****  Rally Results End  **** ");
		Print("***************************** ");
		
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);
		
		foreach (Man player : players)
		{
			string GUID = player.GetIdentity().GetPlainId(); 
			PlayerBase pb = PlayerBase.Cast(player);
			RallyCompetitorData competitor = GetCompetitor(GUID, pb.GetIdentity().GetName());
			
			if (competitor.Finished)
			{
				penalties = RallyGetPenlaty(GUID);
				finalTime = competitor.FinishTime - competitor.StartTime;
				msg = pb.GetIdentity().GetName() + " finished! Time: " + RallyGetTimeString(finalTime) + " Final Time: " + RallyGetTimeString(finalTime + (penalties * RallyPenalty)) + " Penalties x " + penalties +  " [ " + RallyGetCPString(GUID) + "]";
				pb.MessageAction(msg);
			}
		}
	}
	
	void RallyCheckCPs(float timeslice)
	{
		array<Man> players = new array<Man>();
		string msg;
		RallyTimer += timeslice;
		
		if (RallyTimer - RallyResultsTimer >= 60)
		{
			RallyCheckForReset();
			RallyResultsTimer = RallyTimer;
			RallyResultsDump();
		}
		
        GetGame().GetPlayers(players);
		
		foreach (RallyCP cp : RallyCPs)
		{
			foreach (Man player : players)
			{
				PlayerBase pb = PlayerBase.Cast(player);
				vector playerPos = player.GetPosition();
				float distance = vector.Distance(playerPos, cp.Position);

				if (distance <= cp.Radius)
				{
					string GUID = player.GetIdentity().GetPlainId(); 
					RallyCompetitorData competitor = GetCompetitor(GUID, pb.GetIdentity().GetName());
					
					if (!competitor.Finished)
					{
						if (!competitor.Started && cp.CPType == "S")
						{
							competitor.Started = true;
							competitor.StartTime = RallyTimer;
							competitor.FinishTime = 0;
							RallySetCP(GUID, cp.Name);
							msg = pb.GetIdentity().GetName() + " started! Clock is running, good luck! DRIVE CAREFULLY!" + " [ " + RallyGetCPString(GUID) + "]";
							Print("**** Rally: " + msg);
							pb.MessageAction(msg);
							RallyAdminMSG(msg);
						}
						
						if(competitor.Started && cp.CPType != "S" && cp.CPType != "F")
						{
							if (!RallyCheckCP(GUID, cp.Name))
							{
								RallySetCP(GUID, cp.Name);
								float cpTime = RallyTimer - competitor.StartTime;
								msg = pb.GetIdentity().GetName() + " passed " + cp.Name + " Time: " + RallyGetTimeString(cpTime) + " [ " + RallyGetCPString(GUID) + "]";
								Print("**** Rally: " + msg);
								pb.MessageAction(msg);
								RallyAdminMSG(msg);
							}
						}
						
						if(competitor.Started && cp.CPType == "F")
						{
							competitor.Finished = true;
							competitor.FinishTime = RallyTimer;
							RallySetCP(GUID, cp.Name);
							int penalties = RallyGetPenlaty(GUID);
							float finalTime = competitor.FinishTime - competitor.StartTime;
							msg = pb.GetIdentity().GetName() + " finished! Time: " + RallyGetTimeString(finalTime) + " Final Time: " + RallyGetTimeString(finalTime + (penalties * RallyPenalty)) + " Penalties x " + penalties +  " [ " + RallyGetCPString(GUID) + "]";
							Print("**** Rally: " + msg);
							pb.MessageAction(msg);
							RallyAdminMSG(msg);
						}
					}
				}
			}
		}
	}
	
	void RallyCheckForReset()
	{
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);
		
		foreach (Man player : players)
		{
			string GUID = player.GetIdentity().GetPlainId();
			
			if (RallyAdmins.Find(GUID) >= 0)
			{
				PlayerBase pb = PlayerBase.Cast(player);
				vector playerPos = player.GetPosition();
				float distance = vector.Distance(playerPos, RallyResetPos);
				
				if (distance <= RallyResetRadius)
				{
					RallyCompetitors = new array<ref RallyCompetitorData>();
					pb.MessageImportant("!!!RALLY RESET!!!");
				}
			}
		}
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		RallyCheckCPs(timeslice);
	}
	
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 1.00, 1.00 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}
	
	void SetRandomHealthNew(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 1.00, 1.00 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );

		GetGame().SelectPlayer( identity, m_player );

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemClothing;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		player.RemoveAllItems();
		
		array<string> helmets = {
				"DirtBikeHelmet_Black",
				"DirtBikeHelmet_Blue",
				"DirtBikeHelmet_Chernarus",
				"DirtBikeHelmet_Green",
				"DirtBikeHelmet_Khaki",
				"DirtBikeHelmet_Red"
			};
		
		array<string> tops = {
				"TrackSuitJacket_Black",
				"TrackSuitJacket_Blue",
				"TrackSuitJacket_Green",
				"TrackSuitJacket_LightBlue",
				"TrackSuitJacket_Red"
			};
			
		array<string> bottoms = {
				"TrackSuitPants_Black",
				"TrackSuitPants_Blue",
				"TrackSuitPants_Green",
				"TrackSuitPants_LightBlue",
				"TrackSuitPants_Red"
			};
			
		array<string> shoes = {
				"JoggingShoes_Black",
				"JoggingShoes_Blue",
				"JoggingShoes_Red",
				"JoggingShoes_Violet",
				"JoggingShoes_White",
				"AthleticShoes_Blue",
				"AthleticShoes_Black",
				"AthleticShoes_Brown",
				"AthleticShoes_Green",
				"AthleticShoes_Grey"
			};
			
		player.GetInventory().CreateInInventory(helmets.GetRandomElement());
		player.GetInventory().CreateInInventory(bottoms.GetRandomElement());
		player.GetInventory().CreateInInventory(tops.GetRandomElement());
		player.GetInventory().CreateInInventory(shoes.GetRandomElement());
		player.GetInventory().CreateInInventory("TacticalGloves_Black");
		player.GetInventory().CreateInInventory("AviatorGlasses");
		
		// top
		itemClothing = player.FindAttachmentBySlotName( "Body" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );

			itemEnt = itemClothing.GetInventory().CreateInInventory( "BandageDressing" );
			
			if ( Class.CastTo( itemBs, itemEnt ) )
			{
				SetRandomHealthNew( itemEnt );
				itemBs.SetCleanness( 1 );
			}
			
			itemEnt = itemClothing.GetInventory().CreateInInventory( "DirtBikeHelmet_Mouthguard" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "DirtBikeHelmet_Visor" );
			SetRandomHealthNew( itemEnt );
			
			itemEnt = itemClothing.GetInventory().CreateInInventory( "PersonalRadio" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "Battery9V" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "Battery9V" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "Flashlight" );
			SetRandomHealthNew( itemEnt );
		}

		// pants
		itemClothing = player.FindAttachmentBySlotName( "Legs" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );

			/*
			itemEnt = itemClothing.GetInventory().CreateInInventory( "ChernarusMap" );
			SetRandomHealthNew( itemEnt );
			player.SetQuickBarEntityShortcut( itemEnt, 0 );
			*/
			
			itemEnt = itemClothing.GetInventory().CreateInInventory( "TunaCan" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "TunaCan" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "TunaCan" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "TunaCan" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "SodaCan_Cola" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "SodaCan_Cola" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "SodaCan_Cola" );
			SetRandomHealthNew( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "SodaCan_Cola" );
			SetRandomHealthNew( itemEnt );
		}

		// shoes
		itemClothing = player.FindAttachmentBySlotName( "Feet" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );
		}
		
		player.GetStatWater().Set( 5000 );
		player.GetStatEnergy().Set( 5000 );
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}