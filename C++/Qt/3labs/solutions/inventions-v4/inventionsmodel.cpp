#include "inventionsmodel.h"

#include <QDebug>

static QVector<Invention> samples = {
	{ Invention::Realm::Physics,   "Костыль", 2020, "Левша, Правша, Косолапый мишка", false, false, QDate::currentDate() },
	{ Invention::Realm::Maths,     "Велосипед", 2015, "Билл Гейтс, Илон Маск, Дмитрий Рогозин", false, true, QDate(2020, 11, 25) },
};

InventionsModel::InventionsModel(QObject* parent): QAbstractTableModel( parent )
{
	for (const auto& sample : samples) {
		inventions << sample;
	}
}

QVariant InventionsModel::data( const QModelIndex &index, int role ) const {
	if( !index.isValid() ) {
		return QVariant();
	}
	if( index.row() < 0 || index.row() >= inventions.size() || index.column() < 0 || index.column() >= MaxColumnCount ) {
		return QVariant();
	}
	if( Qt::DisplayRole != role && Qt::EditRole != role ) {
		return QVariant();
	}
	///qDebug() << "data( index: row " << index.row() << " col " << index.column() << " role " << role << " )";
#if 1
	const Invention& invention = inventions.at(index.row());
	switch( index.column() ) {
	case 0:
		return Invention::realmName(invention.realm());
		break;
	case 1:
		///qDebug() << "  level: " << beast->level;
		return invention.name();
		break;
	case 2:
		///qDebug() << "  element: " << beast->element;
		return invention.year();
		break;
	}
#else
	BeastOrMaster* bom = reinterpret_cast< BeastOrMaster* >( index.internalPointer() );
	if( BeastOrMaster::bomBeast == bom->type ) {
		Beast* beast = static_cast< Beast* >( bom );
		if( !beast->master ) {
			qDebug() << "  error: invalid master pointer";
			return QVariant();
		}
		if( beast->def >= 0 ) {
			switch( index.column() ) {
			case 0:
				return QString( Beast::Defs[ beast->def ].name );
				break;
			case 1:
				///qDebug() << "  level: " << beast->level;
				return beast->level;
				break;
			case 2:
				///qDebug() << "  element: " << beast->element;
				return QString( ElementStr( beast->element ) );
				break;
			case 3:
				{
					QString enemies;
					for( int j = 0; j < beast->enemyCount; ++j )
						enemies += ( /*QString( ", " ) +*/ BeastTypeStr( beast->enemies[ j ] ) + QString( " " ) );
					///qDebug() << "  enemies: " << enemies;
					return enemies;
					break;
				}
			}
		}
		else {
			if( index.column() == 0 ) {
				return Beast::EmptyName;
			}
			else {
				return QVariant();
			}
		}
	} else if( BeastOrMaster::bomMaster == bom->type ) {
		if( 0 == index.column() ) {
			BeastMaster* master = static_cast< BeastMaster* >( bom );
			///qDebug() << "  master: " << master->name();
			return master->name();
		}
		//return "prop " + QString::number( index.column() );
		return QVariant();
	}
#endif
	return QVariant();
}

bool InventionsModel::setData( const QModelIndex &index, const QVariant &value, int role ) {
//	qDebug() << "setData( index: row " << index.row() << " col " << index.column() << " role " << role << " value " << value << " )";
	if( !index.isValid() ) {
		qDebug() << "  setData( warning: invalid index )";
		return false;
	}
	if( role != Qt::EditRole ) {
		qDebug() << "  setData( warning: role " << role << " )";
		return false;
	}
	/*BeastOrMaster* bom = reinterpret_cast< BeastOrMaster* >( index.internalPointer() );
	if( !bom ) {
		qDebug() << "  setData( error: invalid pointer )";
		return false;
	}
	if( BeastOrMaster::bomBeast == bom->type ) {
		Beast* beast = static_cast< Beast* >( bom );
		if( !beast->master ) {
			qDebug() << "  setData( error: invalid master pointer )";
			return false;
		}
		switch( index.column() ) {
		case 0:
			{
				int def = value.toInt();
				if( beast->def == def ) {
					qDebug() << "  not changed";
					break;
				}
				beast->def = def;
				if( def >= 0 ) {
					if( beast->level <= 0 )
						beast->level = 1;
					else if( beast->level > Beast::Defs[ def ].maxLevel )
						beast->level = Beast::Defs[ def ].maxLevel;
					beast->strength = Beast::Defs[ def ].strength[ beast->level - 1 ];
					if( beast->enemyCount > Beast::Defs[ beast->def ].maxEnemyCount )
						beast->enemyCount = Beast::Defs[ beast->def ].maxEnemyCount;
					qDebug() << "  def: " << beast->def << " level " << beast->level << " strength " << beast->strength;
				}
				else {
					qDebug() << "  def " << beast->def << " (reset) ";
					beast->element = Water;
					beast->level = 0;
					beast->strength = 0;
					beast->enemyCount = 0;
					for( int i = 0; i < Beast::MaxEnemyCount; i++ ) {
						beast->enemies[ i ] = UnkBeastType;
					}
				}
				break;
			}
		case 1:
			if( beast->def < 0 ) {
				qDebug() << "  setData( error: editing level of empty beast: def = " << beast->def << " )";
				break;
			}
			beast->level = value.toInt();
			if( beast->level <= 0 )
				beast->level = 1;
			if( beast->def >= 0 ) {
				int def = beast->def;
				if( beast->level > Beast::Defs[ def ].maxLevel )
					beast->level = Beast::Defs[ def ].maxLevel;
				beast->strength = Beast::Defs[ def ].strength[ beast->level - 1 ];
			}
			else
				beast->strength = 0;
			qDebug() << "  setData: level: " << beast->level << " def " << beast->def << " strength " << beast->strength;
			break;
		case 2:
			{
				if( beast->def < 0 ) {
					qDebug() << "  setData( error: editing element of empty beast: def = " << beast->def << " )";
					break;
				}
				int element = value.toInt();
				qDebug() << "  setData(1): element: " << element;
				if( element < UnkBeastElem )
					element = UnkBeastElem;
				if( element > Ground )
					element = Ground;
				beast->element = static_cast< Elements >( element );
				qDebug() << "  setData(2): element: " << beast->element;
			}
			break;
		case 3:
			{
				if( beast->def < 0 ) {
					qDebug() << "  setData( error: editing enemies of empty beast: def = " << beast->def << " )";
					break;
				}
				QStringList lstEnemies = value.toStringList();
				qDebug() << "  beast: " << Beast::Defs[ beast->def ].name << " maxEnemyCount = " << Beast::Defs[ beast->def ].maxEnemyCount;
				qDebug() << "  enemies: " << lstEnemies << ":";
				if( lstEnemies.size() > Beast::Defs[ beast->def ].maxEnemyCount )
					for( int x = 0; x < lstEnemies.size() - Beast::Defs[ beast->def ].maxEnemyCount; ++x )
						lstEnemies.pop_back();
				beast->enemyCount = lstEnemies.size();
				int j = 0;
				foreach( QString enemy, lstEnemies ) {
					beast->enemies[ j ] = static_cast< BeastTypes >( enemy.toInt() );
					qDebug() << "  [" << j << "]: " << beast->enemies[ j ];
					++j;
				}
				break;
			}
		}
	} else if( BeastOrMaster::bomMaster == bom->type ) {
		qDebug() << "  setData( error: master not editible )";
		return false;
	}
	emit dataChanged( index, index );
	return true;*/
	return false;
}

Qt::ItemFlags InventionsModel::flags( const QModelIndex &index ) const {
	if( !index.isValid() ) {
		return Qt::ItemIsEnabled;
	}
	Qt::ItemFlags flags = QAbstractItemModel::flags( index );
//	qDebug() << "flags( index: row " << index.row() << " col " << index.column() << " ): " << flags;
	/*BeastOrMaster* bom = reinterpret_cast< BeastOrMaster* >( index.internalPointer() );
	if( !bom ) {
		qDebug() << "  flags( error: invalid pointer )";
		return flags;
	}
	if( BeastOrMaster::bomBeast == bom->type ) {
		Beast* beast = static_cast< Beast* >( bom );
		if( !beast->master ) {
			qDebug() << "  flags( error: invalid master pointer )";
			return flags;
		}
		switch( index.column() ) {
		case 0:
			flags |= Qt::ItemIsEditable;
			break;
		case 1: // level
			if( beast->def >= 0 )
				flags |= Qt::ItemIsEditable;
			break;
		case 2:
			if( beast->def >= 0 )
				flags |= Qt::ItemIsEditable;
			break;
		case 3:
			if( beast->def >= 0 )
				flags |= Qt::ItemIsEditable;
			break;
		}
	}*/
	return flags;
}

QVariant InventionsModel::headerData( int section, Qt::Orientation orientation, int role ) const {
	QVariant result;
	qDebug() << "headerData( section " << section << " orientation " << orientation << " role " << role << " )";
	if( Qt::DisplayRole == role ) {
		if( Qt::Horizontal == orientation ) {
			switch( section ) {
			case 0:
				result = tr( "Имя" );
				break;
			case 1:
				result = tr( "Уровень" );
				break;
			case 2:
				result = tr( "Стихия" );
				break;
			case 3:
				result = tr( "Враги" );
				break;
			}
		} else if( Qt::Vertical == orientation ) {
			result = QString::number( section );
		}
		//qDebug() << "  result " << result;
	}
	return result;
}

/*QModelIndex InventionsModel::index( int row, int column, const QModelIndex &parent ) const {
	BeastMaster* master = 0;
	///qDebug() << "index( row " << row << " col " << column << " parent: valid " << parent.isValid() << " )";
	if ( !hasIndex( row, column, parent ) )
		return QModelIndex();
	if( row < 0 || column > MaxColumnCount )
		return QModelIndex();
	if( parent.isValid() ) {
		master = reinterpret_cast< BeastMaster* >( parent.internalPointer() );
		///qDebug() << "  parent [" << master << "]: row " << parent.row() << " col " << parent.column();
		if( !master ) {
			qDebug() << "  error: invalid master pointer";
			return QModelIndex();
		}
		///qDebug() << "  beasts count: " << master->beastCount();
		if( row >= master->beastCount() )
			return QModelIndex();
		Beast* beast = master->beast( row );
		if( !beast ) {
			qDebug() << "  error: invalid beast pointer";
			return QModelIndex();
		}
		///qDebug() << "  create beast index(" << row << ", " << column << ") for [" << beast << "] (" << Beast::Defs[ beast->def ].abbr << ")";
		return createIndex( row, column, beast );
	} else {
		///qDebug() << "  no parent, masters count: " << beastMasters.size();
		if( row >= beastMasters.size() || column > 3 )
			return QModelIndex();
		master = beastMasters.value( row );
		if( !master ) {
			qDebug() << "  error: invalid master pointer";
			return QModelIndex();
		}
		///qDebug() << "  create master index(" << row << ", " << column << ") for [" << master << "] " << master->name();
		return createIndex( row, column, master );
	}
	qDebug() << "  strange place";
	return QModelIndex();
}*/

/*QModelIndex InventionsModel::parent( const QModelIndex &index ) const {
	///qDebug() << "parent( valid " << index.isValid() << " row " << index.row() << " col " << index.column() << " )";
	if( !index.isValid() ) {
		qDebug() << "  warning: invalid index";
		return QModelIndex();
	}
	BeastOrMaster* bom = reinterpret_cast< BeastOrMaster* >( index.internalPointer() );
	///qDebug() << "  bom " << bom;
	if( !bom ) {
		qDebug() << "  error: invalid pointer";
		return QModelIndex();
	}
	if( BeastOrMaster::bomBeast == bom->type ) {
		Beast* beast = static_cast< Beast* >( bom );
		///qDebug() << "  beast: " << Beast::Defs[ beast->def ].abbr;
		if( !beast->master ) {
			qDebug() << "  error: invalid master pointer";
			return QModelIndex();
		}
		int row = beastMasters.indexOf( beast->master );
		if( row < 0 ) {
			qDebug() << "  error: row " << row;
			return QModelIndex();
		}
		///qDebug() << "  create parent index: row " << row;
		return createIndex( row, 0, beast->master );
	} else if( BeastOrMaster::bomMaster == bom->type ) {
		///qDebug() << "  master: " << static_cast< BeastMaster* >( bom )->name();
		return QModelIndex();
	}
	return QModelIndex();
}*/

int InventionsModel::rowCount( const QModelIndex& /*parent*/ ) const {
	///qDebug() << "rowCount( parent: valid " << parent.isValid() << " row " << parent.row() << " col " << parent.column() << " )";
	#if 1
	return inventions.count();
	#else
	int result = 0;
	if( parent.isValid() ) {
		BeastOrMaster* bom = reinterpret_cast< BeastOrMaster* >( parent.internalPointer() );
		if( BeastOrMaster::bomBeast == bom->type ) {
			///qDebug() << "  beast: " << Beast::Defs[ static_cast< Beast* >( bom )->def ].abbr;
			result = 0;
		} else if( BeastOrMaster::bomMaster == bom->type ) {
			BeastMaster* master = static_cast< BeastMaster* >( bom );
			///qDebug() << "  master [" << master << "]: " << master->name();
			result = master->beastCount();
		}
	} else {
		result = beastMasters.count();
	}
	///qDebug() << "  result " << result;
	return result;
#endif
}

int InventionsModel::columnCount( const QModelIndex &/*parent*/ ) const {
//	qDebug() << "columnCount " << MaxColumnCount;
	return MaxColumnCount;
}

