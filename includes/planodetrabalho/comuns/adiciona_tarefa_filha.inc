<?php

//$strTarefaClassName = 'tarefaPT';
//$strProjetoClassName = 'projetoPT';

$intTarefaId 	= @$_REQUEST[ 'rsargs' ][ 2 ];
$intProjetoId	= @$_REQUEST[ 'rsargs' ][ 3 ];

$objTarefa = new $strTarefaClassName();
//$objTarefa = new Tarefa(); // ME REMOVA AGORA!!
$objTarefa->setNome( 'Nova Tarefa' );
$objTarefa->setDescricao( 'Nova Descrição');
$objTarefa->setProjetoId( $intProjetoId );
if( $intTarefaId > 0 )
{
	$objTarefa->setContainerId( $intTarefaId );
	$objTarefa->inserirTarefa();
}
AbstractEntity::updateAllChangedEntities();
$db->commit();;

?>