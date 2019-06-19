<?php
class Par3_Model_AnalisePlanejamentoIniciativa extends Modelo
{
	/*
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	private $anaid;
	protected $stNomeTabela = "par3.analise";

	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('anaid');

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
			'anaid'            => null,
			'inpid'           => null,
			'anaano'        => null,
			'anaconsideracaoent'      => null,
			'anaconsideracaoprop'    => null,
			'anaconsideracaoproj'   => null,
			'anaconsideracaoobj'  => null,
			'anaconsideracaojus' => null,
			'anaconsideracaoval' => null,
			'anaoutrasconsideracoes' => null,
			'anaparecer' => null,
			'anatextoparecer' => null,
			'cpftecnico' => null,
			'cpfcoordenador' => null,
			'docid' => null,
			'anadatacriacao' => null,
			'anadataalteracao' => null,
			'anastatus' => null,
	);

	public function __construct( $anaid )
	{
		$this->anaid = $anaid;
	}
	
	public function salvar($arrPost)
	{
	    
		if( $arrPost['form-item-composicao']['qtd-item-aprovado'] ){
		    $sql = "update par3.analise_itemcomposicao set aicstatus = 'I' where anaid = {$arrPost['anaid']}";
		    $this->executar($sql);
		    
			foreach ($arrPost['form-item-composicao']['qtd-item-aprovado'] as $iigid => $v ) {
				
				//$ipiaprovado = $v[$arrPost['iniano']];
				
				$qtd_item = $arrPost['form-item-composicao']['qtd-item'][$iigid][$arrPost['iniano']];
				$ipiid = $arrPost['form-item-composicao']['ipiid'][$iigid];
				$qtd_item_emenda = $arrPost['form-item-composicao']['qtd-item-emenda'][$iigid][$arrPost['iniano']];
				$valor_referencia = $arrPost['form-item-composicao']['valor_referencia'][$iigid][$arrPost['iniano']];
				
				$qtd_item = ( !empty($qtd_item_emenda) ? $qtd_item_emenda : $qtd_item);
				////$radioItem = array('N' => 'Não', 'S' => 'Sim', 'SS' => 'Sim com Saldo');
				$ipiquantidadeaprovada = $arrPost['form-item-composicao']['qtd-item-aprovado'][$iigid][$arrPost['iniano']];
				
				if( (int)$qtd_item == (int)$ipiquantidadeaprovada ){
				    $ipiaprovado = 'S';
				}elseif( (int)$ipiquantidadeaprovada  > 0 && (int)$qtd_item > (int)$ipiquantidadeaprovada ){
				    $ipiaprovado = 'CS';
				} else {
				    $ipiaprovado = 'N';
				}
				
				$iniano = $arrPost['form-item-composicao']['iniano'][$iigid];

				if(!$valor_referencia){
                    $valor_referencia = 0;
                }
				
				$sql = "INSERT INTO par3.analise_itemcomposicao(ipiid, anaid, aicqtdaprovado, aicvaloraprovado, usucpf, aicaprovado)
                        VALUES($ipiid, {$arrPost['anaid']}, {$ipiquantidadeaprovada}, {$valor_referencia}, '{$_SESSION['usucpf']}', '{$ipiaprovado}')";
				$this->executar($sql);
				$this->commit();
			}
		}
		
		$inpid = ($arrPost['form-item-composicao']['inpid'][0] ? $arrPost['form-item-composicao']['inpid'][0] : $_REQUEST['inpid']);
		$anaid = $arrPost['anaid'];
		$iniano = ($arrPost['ano_iniciativa'] ? $arrPost['ano_iniciativa'] : $_REQUEST['iniano']);
		$anaconsideracaoent = ($arrPost['anaconsideracaoent']);
		$anaconsideracaoprop = ($arrPost['anaconsideracaoprop']);
		$anaconsideracaoproj = ($arrPost['anaconsideracaoproj']);
		$anaconsideracaoobj = ($arrPost['anaconsideracaoobj']);
		$anaconsideracaojus = ($arrPost['anaconsideracaojus']);
		$anaconsideracaoval = ($arrPost['anaconsideracaoval']);
		$anaoutrasconsideracoes = ($arrPost['anaoutrasconsideracoes']);
		$anatextoparecer = ($arrPost['anatextoparecer']);
		$anaparecer = $arrPost['anaparecer'];
		$docid = $arrPost['docid'];
		$arRetorno = array();
		
		if( !empty($anaid) ){
			$sql = "UPDATE par3.analise SET
						anaconsideracaoent = '{$anaconsideracaoent}', anaconsideracaoprop = '{$anaconsideracaoprop}',
				       	anaconsideracaoproj = '{$anaconsideracaoproj}', anaconsideracaoobj = '{$anaconsideracaoobj}', anaconsideracaojus = '{$anaconsideracaojus}',
				       	anaconsideracaoval = '{$anaconsideracaoval}', anaoutrasconsideracoes = '{$anaoutrasconsideracoes}', anaparecer = '{$anaparecer}',
				       	anatextoparecer = '{$anatextoparecer}', cpftecnico = '{$_SESSION['usucpf']}',
				       	anadataalteracao = 'now()'
				 	WHERE anaid = $anaid";
			$this->executar($sql);
		} else {
			$sql = "INSERT INTO par3.analise(inpid, anaano, docid, anaconsideracaoent, anaconsideracaoprop, anaconsideracaoproj, anaconsideracaoobj, anaconsideracaojus, anaconsideracaoval,
						anaoutrasconsideracoes, anaparecer, anatextoparecer, cpftecnico, cpfcoordenador, anadatacriacao, anadataalteracao, anastatus, intaid)
	    			VALUES ($inpid, $iniano, $docid, '{$anaconsideracaoent}', '{$anaconsideracaoprop}', '{$anaconsideracaoproj}', '{$anaconsideracaoobj}', '{$anaconsideracaojus}', '{$anaconsideracaoval}',
	    			'{$anaoutrasconsideracoes}', '{$anaparecer}', '{$anatextoparecer}', '{$_SESSION['usucpf']}', null, 'now()', null, 'A', 1) returning anaid";
			$anaid = $this->pegaUm($sql);
		}
		$this->commit();
		return $anaid;
	}
	
	public function carregarAnalisePlanejamento($anaid){
		$sql = "SELECT anaid, inpid, anaano, anaconsideracaoent, anaconsideracaoprop, docid,
					anaconsideracaoproj, anaconsideracaoobj, anaconsideracaojus,
			       	anaconsideracaoval, anaoutrasconsideracoes, anaparecer, anatextoparecer,
			       	cpftecnico, cpfcoordenador, anadatacriacao, anadataalteracao, anastatus, edeid, intaid
			  	FROM par3.analise
				WHERE anaid = $anaid";
		$arDados = $this->pegaLinha($sql);
		
		return $arDados;
	}
	
	public function carregarAnalisePlanejamentoDiligencia($inpid, $boolEsdidDiligencia = true ){

	    if( $boolEsdidDiligencia ){
	        $andEsdidDiligencia = " AND d.esdid = ".PAR3_ESDID_ANALISE_DILIGENCIA." ";
        }
		$sql = "SELECT a.anaid, a.inpid, a.anaano, a.anaconsideracaoent, a.anaconsideracaoprop,
				       a.anaconsideracaoproj, a.anaconsideracaoobj, a.anaconsideracaojus,
				       a.anaconsideracaoval, a.anaoutrasconsideracoes, a.anaparecer, a.anatextoparecer,
				       a.cpftecnico, a.cpfcoordenador, a.docid, a.anadatacriacao, a.anadataalteracao,
				       a.anastatus
				FROM par3.iniciativa_planejamento ip
					inner join par3.analise a on a.inpid = ip.inpid and a.anastatus = 'A'
				    inner join workflow.documento d on d.docid = a.docid
				where ip.inpid = $inpid
					{$andEsdidDiligencia}
				order by a.anaano";
		$arDados = $this->carregar($sql);
		
		return $arDados;
	}
	
	public function carregaEsdidAtual( $docid ){
		$sql = "select ed.esdid, ed.esddsc from workflow.documento d
					inner join workflow.estadodocumento ed on ed.esdid = d.esdid
				where docid = $docid";
		$arEstato = $this->pegaLinha($sql);
		return $arEstato;
	}
	
	public function carregaBotaoAcaoWorkFlow( $esdid )
	{
		
		$sql = "select
					a.aedid,
					a.aeddscrealizar,
					a.aeddscrealizada
				from workflow.acaoestadodoc a
					inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
				where
					esdidorigem = $esdid and
					aedstatus = 'A' and
					aedvisivel = true
				order by a.aedordem asc";
		$arWork = $this->carregar($sql);
		$arWork = $arWork ? $arWork : array();
		
		return $arWork;
	}
	
	public function sqlCarregaAnaliseValidacaoCoordenador( $arrPost = array() ){
		
	    $inddsc  = sanitizar_string_pesquisa($arrPost['inddsc']);
	    $inpid   = $arrPost['codigo'];
	    
	    $join = '';
	    $where = '';
	    $where  .= $arrPost['inpid_filtro'] ? "  AND inp.inpid = {$arrPost['inpid_filtro']} ":'';
	    $where  .= $arrPost['iarid'] ? "  AND ({$arrPost['iarid']}) in (SELECT DISTINCT
										                        iar.iarid
										                        FROM par3.iniciativa_iniciativas_areas iiar
										                        INNER JOIN par3.iniciativa_areas iar ON iar.iarid = iiar.iarid
										                        WHERE iniid = ini.iniid) ":'';
	    
	    $where  .= $arrPost['desid'] ? "  AND ({$arrPost['desid']}) in (SELECT DISTINCT
										        des.desid
										        FROM par3.iniciativa_planejamento_desdobramentos ipd
										        INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
										        WHERE ipd.inpid = inp.inpid) ":'';
	    
	    
	    $where  .= $arrPost['inddsc']?" AND upper(removeacento(ind.inddsc)) LIKE '%{$inddsc}%' ":'';
	    $where  .= $arrPost['cicid'] && validarInteiro($arrPost['cicid'])?"AND ini.cicid = {$arrPost['cicid']}":"";
	    $where  .= $arrPost['dimid'] && validarInteiro($arrPost['dimid'])?"AND inp.dimid = {$arrPost['dimid']}":"";
	    $where  .= $arrPost['esdid'] ? " AND docini.esdid = {$arrPost['esdid']}" : ' and docini.esdid in ('.PAR3_ESDID_EM_ANALISE.', '.PAR3_ESDID_AGUARDANDO_ANALISE.', '.PAR3_ESDID_EM_DILIGENCIA.') ';
	    

	    
	    if($arrPost['muncod'] && count($arrPost['muncod']) != 0 ){
	        $arrMuncod = tratarArrayParaMultiSelect($arrPost['muncod']);
	        if(count($arrMuncod) != 0){
	            $where  .= " AND iu.muncod::integer in (".implode(',',$arrMuncod).")";
	        }
	    }
	    
	    if($arrPost['estuf']){
	        $arrEstuf = implode(',',array_map(function($estuf){return (string)"'".$estuf."'";},$arrPost['estuf']));

	        if($arrEstuf != "''"){
	            $where  .= $arrPost['estuf'] ? " AND iu.estuf in ({$arrEstuf})":'';
	        }
	        
	    }
	    
	    if($arrPost['etaid']){
	        $where .= " AND eta.etaid = {$arrPost['etaid']}";
	        $join .= " INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid ";
	    }

	    $where  .= $arrPost['intaid'] ? " AND inta.intaid = {$arrPost['intaid']}":'';
	    $where  .= $arrPost['intoid'] ? " AND obj.intoid = {$arrPost['intoid']}":'';
	    $where  .= $arrPost['indid'] ? " AND ini.iniid = {$arrPost['indid']}":'';
	    $where  .= $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? "AND iu.itrid in(1,3)" :"AND iu.itrid = {$arrPost['itrid']}"): '';
	    
	    
	    if( $arrPost['tipoassistencia'] ){
	        $where  .= " AND tpas.intaid = {$arrPost['tipoassistencia']} ";
	        $join .= " inner join par3.iniciativa_tipos_assistencia tpas on tpas.intaid = a.intaid"; 
	    }
	    
	    if( $arrPost['iniano'] ){
	        $where  .= " AND iia.iniano = {$arrPost['iniano']}";
	        $join .= " inner join par3.iniciativa_iniciativas_anos iia on iia.iniid = inp.iniid ";
	    }
	    if( $arrPost['prgid'] ){
	        $where  .= " AND inip.prgid = {$arrPost['prgid']}";
	        $join .= ' inner join par3.iniciativa_iniciativas_programas inip on inip.iniid = ini.iniid ';
	    }
	    if( $arrPost['usucpf_analise'] ){
	        $where .= " AND a.cpftecnico = '{$arrPost['usucpf_analise']}'";
	    }
	    if( $arrPost['esdid_analise']){
	        
	        //verificar se o array tem 1 posição e tratar se os valores estiverem separados por vírgula
	        $arrCampo = $arrPost['esdid_analise'];
	        $arrImplode = array();
	        if(is_array($arrCampo)){
	            $arr = array_filter($arrCampo,function($val){return !empty($val);});
	            if(count($arr) == 1){
	                $arrImplode = explode(',',$arr[0]);
	            }
	            if(count($arr) > 1){
	                $arrImplode = $arr;
	            }
	            $_REQUEST['esdid_analise'] = $arrImplode;
	        }
	        $where  .= " AND ed.esdid in (".implode(',',$arrImplode).")";
	        
	    }

	    if( $arrPost['escid'] ){
	        $where .= " AND inp.inpid in (SELECT ip.inpid
										FROM
											par3.iniciativa_planejamento ip
											inner join par3.iniciativa_iniciativas_anos iia on iia.iniid = ip.iniid
											inner join par3.iniciativa_itenscomposicao_grupo iicg on iicg.iniid = iia.iniid
											inner join par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipiano = iia.iniano
										    inner join par3.iniciativa_planejamento_item_composicao_escola ipe on ipe.ipiid = ipc.ipiid and ipe.ipestatus = 'A'
										WHERE ipc.ipistatus = 'A'
											and ipe.escid = {$arrPost['escid']}) ";
	    }
	    
	    $contador = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls'? "ROW_NUMBER() OVER ({$orderBy}) AS qtd," : '';
	    $inicio   = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls'? "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (":'';
	    $fim      = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls'? ") as foo":'';
	    

	    $sql = "{$inicio}
				SELECT
                        '<input type=\"checkbox\" id=\"checklote['||a.anaid||']\" name=\"checklote\" value=\"'||a.anaid||'\" class=\"checkbox_lote\">' as acao, 
                        a.anaid as analise,
                        inp.inpid as planejamento,
                        a.anaano as ano,   
                        iu.estuf as estado, 
                        iu.inudescricao as entidade,           
                        ini.iniid ||' - '|| idesc.inddsc as iniciativa,
                        obj.intodsc as objeto, 
                        CASE  WHEN a.intaid = 1
                            THEN 'PAR'
                            ELSE 'Emendas'
                        END as Tipo_assistencia,
                        sum(aic.aicvaloraprovado*aic.aicqtdaprovado) AS valor_total_aprovado,
                        ed.esddsc as situacao_analise,
                        coalesce(u.usunome, '-') as usunome,
                        coalesce(to_char(a.anadatacriacao, 'DD/MM/YYYY'), '-') as dataanalise
                FROM par3.iniciativa_planejamento inp
                INNER JOIN par3.analise a ON a.inpid = inp.inpid AND a.anastatus = 'A'
                INNER JOIN par3.instrumentounidade iu ON iu.inuid = inp.inuid   
                INNER JOIN workflow.documento docini ON docini.docid = inp.docid 
                INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = ini.intoid
                INNER JOIN par3.iniciativa_descricao idesc ON idesc.indid = ini.indid        
                INNER JOIN workflow.documento doc ON doc.docid = a.docid
                INNER JOIN workflow.estadodocumento ed ON doc.esdid = ed.esdid                    
                INNER JOIN par3.analise_itemcomposicao aic ON aic.anaid = a.anaid AND aic.aicstatus = 'A' 
                inner join seguranca.usuario u on u.usucpf = a.cpftecnico
                {$join}
                WHERE inp.inpstatus = 'A'::bpchar 
                    {$where}
                GROUP BY docini.esdid, ed.esdid, a.anaid, a.anaano, inp.inpid,iu.inuid, iu.estuf, iu.inudescricao, ini.iniid,  idesc.inddsc, obj.intodsc,  ed.esddsc,a.intaid, u.usunome,a.anadatacriacao
                ORDER BY iu.estuf, iu.inudescricao, inp.inpid

                    {$fim}
                ";
        
		return $sql;
	}
	
	public function verificaExisteAnalisePlanejamento( $inpid, $ano ){
		$sql = "select a.anaid from par3.analise a where a.inpid = $inpid and a.anaano = $ano";
		$anaid = $this->pegaUm($sql);
	
		return $anaid;
	}
}