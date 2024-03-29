<?php

	
class SupervisaoFNDE extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisaofnde";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sfndeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sfndeid' => null, 
									  	'sobid' => null, 
									  	'entidvistoriador' => null, 
									  	'endid' => null, 
									  	'docid' => null, 
									  	'empid' => null, 
									  	'usucpf' => null, 
									  	'efndeid' => null, 
									  	'sfndedtsupervisao' => null, 
									  	'sfndedtcadastro' => null, 
									  	'sfndefuncionamento' => null, 
									  	'sfndecargorepresentante' => null, 
									  	'sfndestatus' => null, 
									  	'sfndeendcorreto' => null, 
									  	'sfndeacordo' => null, 
									  	'sfndedtatualizacao' => null, 
									  );

    public function pegaNomeResponsavelPorSueid( $sfndeid ){
        $sql = "SELECT
                                entnome
                        FROM
                                obras2.supervisaofnde se
                        JOIN entidade.entidade e ON e.entid = se.entidvistoriador
                        WHERE
                                sfndeid = {$sfndeid}";

        return $this->pegaUm( $sql );
    }

    public function pegaSupidByObraAndSfndeid($empid, $sfndeid) {
        if (empty($empid) || empty($sfndeid)) {
            return '';
        }

        $sql = "SELECT
                            su.supid
                    FROM
                            obras2.supervisaofnde s
                    JOIN obras2.supervisao su ON su.sfndeid = s.sfndeid AND su.supstatus = 'A'
                    WHERE
                            sfndestatus = 'A' AND
                            s.empid = {$empid} AND
                            s.sfndeid = {$sfndeid}";

        return $this->pegaUm($sql);
    }


    public function listaSql( array $param = array() ){
		$where   = array();
		$where[] = "sfndestatus = 'A'";
		
		
		if( $param['empid'] ){
			$param['empid'] = (array) $param['empid'];
			$where[] = "sm.empid IN(" . implode(", ", $param['empid']) . ")";
		}

		if( $param['obrid'] ){
			$join[] = "INNER JOIN obras2.obras obr ON obr.empid = sm.empid AND obr.obrid = {$param['obrid']}";
		}
		
		if( $param['empid'] )
			$ultSfndeid 	  = $this->pegaUltSfndeidByEmpid( current($param['empid']) );
		$ultSfndeid	  = ($ultSfndeid ? $ultSfndeid : 0);
		$cpf	  	  = $_SESSION['usucpf'];
		$acessoPerfil = (possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR) ) ? 'true' : 'false');
		
		$imgExcAtiva = "<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/excluir.gif\"
		 					style=\"cursor: pointer; margin-left: 3px;\"
		 					onclick=\"javascript: excluirSupFNDE(' || sm.sfndeid || ');\"
		 					title=\"Excluir Superviso\">";
		
		$imgExcInativa = "<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/excluir_01.gif\"
		 					style=\"cursor: pointer; margin-left: 3px;\"
		 					onclick=\"javascript: alert(''Esta superviso no pode ser excluda'');\"
		 					title=\"Esta superviso no pode ser excluda\">";
		
		$acao = "'<center><div style=\"width:80px\">
							<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/alterar.gif\"
			 					style=\"cursor: pointer\"
			 					onclick=\"javascript: alterarSupFNDE(' || sm.sfndeid || ');\"
			 					title=\"Alterar Superviso\">' ||
			 				CASE 
			 					WHEN (sm.sfndeid = {$ultSfndeid}) AND (sm.usucpf = '{$cpf}' OR $acessoPerfil) THEN
			 						'{$imgExcAtiva}'
			 					ELSE
			 						'{$imgExcInativa}'
			 				END || '

			 				<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/print.png\"
			 					style=\"cursor: pointer\"
			 					onclick=\"javascript: imprimirQuestionarioRespondidoFNDE(' || sm.sfndeid || ');\"
			 					title=\"Imprimir questionrio respondido\">

					</div></center>'";

        $join = (is_array($join) ? $join : array());
		$sql = "SELECT
					{$acao} AS acao,
					ed.esddsc,
					TO_CHAR(sm.sfndedtsupervisao, 'dd-mm-YYYY') AS sfndedtsupervisao,
					TO_CHAR(sm.sfndedtcadastro, 'dd-mm-YYYY') AS sfndedtcadastro,
					e.entnome,
					sm.sfndecargorepresentante,
					u.usunome,
					CASE 
						WHEN sfndefuncionamento = 's' THEN 'Sim' 
						WHEN sfndefuncionamento = 'n' THEN 'No'
						ELSE ''
					END AS sfndefuncionamento 
				FROM
					obras2.supervisaofnde sm
				LEFT JOIN workflow.documento d ON d.docid = sm.docid
				LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				LEFT JOIN entidade.entidade e ON e.entid = sm.entidvistoriador
				JOIN seguranca.usuario u ON u.usucpf = sm.usucpf ".
				(count($join) ? implode(' ',$join) : "") . "
				WHERE
					" . (count($where) ? implode(' AND ',$where) : "");
		
		return $sql;
	} 
	
	public function pegaUltSfndeidByEmpid( $empid ){
		$sql = "SELECT
					MAX(sfndeid) AS sfndeid
				FROM
					obras2.supervisaofnde
				WHERE
					sfndestatus = 'A' AND
					empid = {$empid}";
		
		return $this->pegaUm( $sql );
	}	
}
