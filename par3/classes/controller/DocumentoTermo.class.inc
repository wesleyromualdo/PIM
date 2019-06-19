<?php
class Par3_Controller_DocumentoTermo extends Controle
{
	private $model;
	private $countHtml;
	private $valorTermo;
	private $arrTipoObjetoTermo = array(2, 3); #3 - Evento, 2 - Formação;

	public function __construct()
	{
		$this->model = new Par3_Model_DocumentoTermo();
		$this->countHtml = 0;
	}
	
	public function salvar_termo( $arrPost = array() ){
		$arrPost['usucpf'] = $_SESSION['usucpf'];
		unset($arrPost['dotvalortermo']);
		if( $arrPost['tipo_processo'] == 'O' ){
			$tipo_processo = 'OBRA';
			$sql = "select p.proid, p.obrid, o.obrano, i.intoid, ip.inpid
					from par3.processoobracomposicao p
						inner join par3.obra o on o.obrid = p.obrid and o.obrstatus = 'A'
					    inner join par3.iniciativa_planejamento ip on ip.inpid = o.inpid
					    inner join par3.iniciativa i on i.iniid = ip.iniid
					where p.pocstatus = 'A'
						and p.proid = {$arrPost['proid']}";
			$arrComposicao = $this->model->carregar($sql);
			$arrComposicao = $arrComposicao ? $arrComposicao : array();
			
			$vlrObra = $this->model->pegaUm("select sum(o.obrvalor) - sum(coalesce(o.obrvalor_contrapartida,0))
                        					from par3.processoobracomposicao p
                        						inner join par3.obra o on o.obrid = p.obrid and o.obrstatus = 'A'
                        					where p.pocstatus = 'A'
                        						and p.proid = {$arrPost['proid']}");
			$arrPost['dotvalortermo'] = $vlrObra;
		} else {
			$tipo_processo = 'PAR';
			$sql = "select p.proid, p.inpid, p.ppcano, a.anaid, i.intoid, a.anaid
					from par3.processoparcomposicao p
						inner join par3.analise a on a.inpid = p.inpid and a.anaano = p.ppcano and a.anastatus = 'A'
					    inner join par3.iniciativa_planejamento ip on ip.inpid = a.inpid
					    inner join par3.iniciativa i on i.iniid = ip.iniid
					where p.ppcstatus = 'A'
						and p.proid = {$arrPost['proid']}";
			$arrComposicao = $this->model->carregar($sql);
			$arrComposicao = $arrComposicao ? $arrComposicao : array();
		}
		$intoid = $arrComposicao[0]['intoid']; #tipo objeto
		
		$arrPost['arChavePrimaria'] 	= $this->model->arChavePrimaria[0];
		$arrPost['stNomeTabela'] 		= $this->model->stNomeTabela;
		$arrPost['arAtributosTabela'] 	= $this->model->arAtributosTabela;
		
		if( $arrPost['dotjustificativa_de_oficio'] )
		    $arrPost['dotjustificativa_de_oficio'] = $arrPost['dotjustificativa_de_oficio'];
        if( $arrPost['dotdata_de_oficio'] )
		    $arrPost['dotdata_de_oficio'] = formata_data_sql($arrPost['dotdata_de_oficio']);
        
		   // ver($arrPost,d);
		if( empty($arrPost['dotid']) ){
		    
		    if( $arrPost['aditivo'] == 'S' || $arrPost['exoficio'] == 'S' ){
		        $sql = "SELECT dotidpai, dotnumero, dotdatainiciovigencia, dotdatafimvigencia, dotdatainclusao FROM par3.documentotermo WHERE dotid = ".($arrPost['dotid_ref'] ? $arrPost['dotid_ref'] : $arrPost['dotid_ex']);
		        $arLinha = $this->model->pegaLinha($sql);
		        
		        $arrPost['docid']                 = $this->pegaIdWorkFlow();
		        
		        $arrPost['dotidpai']              = $arrPost['dotid_ref'];
		        if(  $arrPost['exoficio'] == 'S' ){
		            $arrPost['dotdatafimvigencia']    = $arrPost['dotdata_de_oficio'];
		        } else {
		            $arrPost['refid']                 = $arrPost['refid'];
		            $arrPost['dotdatafimvigencia']    = ($arrPost['dotdatafimvigencia'] ? $arrPost['dotdatafimvigencia'] : $arLinha['dotdatafimvigencia']);
		        }
		        $arrPost['dotnumero']             = $arLinha['dotnumero'];		        
		        $arrPost['tirid']                 = $arrPost['tirid'];		        
		        $arrPost['dotdatainiciovigencia'] =  ($arrPost['dotdatainiciovigencia'] ? $arrPost['dotdatainiciovigencia'] : $arLinha['dotdatainiciovigencia']);
		        $arrPost['dotdatainclusao']       = 'now()';
		        $arrPost['usucpfinclusao']        = $_SESSION['usucpf'];
		    } else {
    			$max = $this->model->pegaUm( "SELECT coalesce(max(dotnumero),0) FROM par3.documentotermo WHERE substring(cast(dotnumero as varchar),1,4) = to_char(now(), 'YYYY') AND char_length(cast(dotnumero as varchar)) > 4" );
    			$dopNumero = $max ? ($max + 1) : (date('Y') . '00001');
    			
    			$arrPost['dotnumero'] = $dopNumero;
    			$arrPost['dotdatainiciovigencia'] = date('d').'/'.date('m').'/'.date('Y');
    			$arrPost['dotdatafimvigencia'] = date('d').'/'.date('m').'/'.(date('Y')+1);
    			$arrPost['dotdatainclusao'] = 'now()';
    			$arrPost['usucpfinclusao'] = $_SESSION['usucpf'];
		    }
		} else {
			$arrPost['dotdataalteracao'] = 'now()';
			$arrPost['usucpfalteracao'] = $_SESSION['usucpf'];
		}
		//$arrPost['dotvalortermo'] = ($_SESSION['par3']['termo']['dotvalortermo'] ? $_SESSION['par3']['termo']['dotvalortermo'] : $arrPost['dotvalortermo']);
		//$arrPost['dotvalortermo'] = ($arrPost['dotvalortermo'] ? $arrPost['dotvalortermo'] : 'null');
		$arrPost['intoid'] = $intoid;
		$arrPost['dotstatus'] = ($arrPost['bloqueado'] == 'S' ? 'B' : 'A');
		
		$contrapartida = $this->pegaValorContrapartidaProcesso($arrPost['proid']);
		$contrapartida = ($contrapartida ? $contrapartida : 0);
		
		/*if( (float)$contrapartida < (int)1 ){
    		$vlr_Total_Empenho = $this->pegaValorTotalEmpenhoProcesso($arrPost['proid']);
    		if( (float)$vlr_Total_Empenho < ((float)$arrPost['dotvalortermo'] ) ){
    		    $contrapartida = ((float)$arrPost['dotvalortermo']  - (float)$vlr_Total_Empenho);
    		}
		}
		$arrPost['dotvalorcontrapartida'] = $contrapartida;*/
		//ver($arrPost,d);
		$dotid = trata_salvar($arrPost);
		
		$arrPostTermo = array();
		$arrPostTermo['arChavePrimaria'] 	= 'tecid';
		$arrPostTermo['stNomeTabela'] 		= 'par3.termocomposicao';
		$arrPostTermo['dotid'] 		= $dotid;
		
		foreach ($arrComposicao as $ter) {
			if( $arrPost['tipo_processo'] == 'O' ){
				$sql = "DELETE FROM par3.termocomposicao WHERE dotid = {$dotid} and obrid = {$ter['obrid']} and tecano = {$ter['obrano']}";
				$this->model->executar($sql);
				
				$arrPostTermo['arAtributosTabela'] 	= array('tecid', 'dotid', 'inpid', 'tecano', 'tecstatus', 'obrid');
				$arrPostTermo['inpid'] 	= $ter['inpid'];
				$arrPostTermo['tecano'] = $ter['obrano'];
				$arrPostTermo['obrid'] = $ter['obrid'];
			} else {
				$sql = "DELETE FROM par3.termocomposicao WHERE dotid = {$dotid} and inpid = {$ter['inpid']} and tecano = {$ter['ppcano']}";
				$this->model->executar($sql);
				
				$arrPostTermo['arAtributosTabela'] 	= array('tecid', 'dotid', 'inpid', 'tecano', 'tecstatus', 'anaid');
				$arrPostTermo['inpid'] 	= $ter['inpid'];
				$arrPostTermo['tecano'] = $ter['ppcano'];
				$arrPostTermo['anaid'] = $ter['anaid'];
			}
			trata_salvar($arrPostTermo);
		}
		
		$this->model->commit();
		
		if( $arrPost['requisicao'] == 'carregar-termo' ){
		    $arrPost['mdoconteudo_t'] = str_replace("\'", '"', $arrPost['mdoconteudo_t']);
		    $arrPost['mdoconteudo_t'] = str_replace('\"', '"', $arrPost['mdoconteudo_t']);
		    $this->gravaHtmlDocumento( $arrPost['mdoconteudo_t'], $dotid, $arrPost['proid'], $tipo_processo );
		    
			return $dotid;
		} else {
			$arrPost['mdoconteudo_t'] = str_replace("\'", '"', $arrPost['mdoconteudo_t']);
			$arrPost['mdoconteudo_t'] = str_replace('\"', '"', $arrPost['mdoconteudo_t']);
			$this->gravaHtmlDocumento( $arrPost['mdoconteudo_t'], $dotid, $arrPost['proid'], $tipo_processo );
			
			if( $arrPost['aditivo'] == 'S' && $arrPost['refid'] ){
			    $url = "par3.php?modulo=principal/planoTrabalho/planejamento/listaReformulacao&acao=A&proid=".$arrPost['proid'];
			    require_once APPRAIZ . 'includes/workflow.php';
			    
			    $docid_Ref = $this->model->pegaUm("SELECT docid FROM par3.reformulacao_documento WHERE refid = {$arrPost['refid']}");
			    
			    wf_alterarEstado( $docid_Ref, $arrPost['aedid'], 'Fluxo de Reformulação', array('refid' => $arrPost['refid'], 'dotid' => $arrPost['dotid_original'], 'proid' => $arrPost['proid']) );
			    
			    $this->model->executar("UPDATE par3.documentotermo SET dotstatus = 'I' WHERE dotid = {$arrPost['dotid_original']}");
			    $this->model->executar("UPDATE par3.documentotermo SET dotstatus = 'A' WHERE dotid = {$dotid}");
			    $this->model->commit();
			    
			} else {
                $url = 'par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A&proid='.$arrPost['proid'].'&tipo_processo='.$arrPost['tipo_processo'];
			}
			if( $arrPost['requisicao'] == 'salvar-termo' ){
			    $url = ($arrPost['url'] ? $arrPost['url'] : $url);
				if( (int)$dotid > 0 ){
					simec_redirecionar($url, 'success', 'Documento Gerado com Sucesso');
				} else {
					simec_redirecionar($url, 'error', 'Erro ao Gerar o Documento');
				}
			}
		}
	}
	
	public function listar( $arrPost = array(), $tipoVisualizacao='html'){
	    $where = '';
	    if($arrPost['muncod'] && count($arrPost['muncod']) != 0 ){
	        $arrMuncod = array_filter($arrPost['muncod'],function($val){return !empty($val);});
	        if(count($arrMuncod) != 0){
	            $where  .= " AND iu.muncod::integer in (".implode(',',$arrMuncod).")";
	        }
	    }
	    $where  .= $arrPost['estuf'] ? " AND iu.estuf = '{$arrPost['estuf']}'":'';
	    $where  .= $arrPost['proid'] ? " AND p.proid = '{$arrPost['proid']}'":'';
	    $where  .= $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? "AND iu.itrid in(1,3)" :"AND iu.itrid = {$arrPost['itrid']}"): "";
        $where  .= $arrPost['intoid'] ? " AND obj.intoid = {$arrPost['intoid']}":'';

        if($arrPost['dotnumero']) {
            $dotnumero = strstr($arrPost['dotnumero'],'-',true);
            $dotnumero = $dotnumero?trim($dotnumero):$arrPost['dotnumero'];
            if(filter_var($dotnumero,FILTER_VALIDATE_INT)){
                $where  .= $dotnumero ? " AND dt.dotnumero::text ilike '%{$dotnumero}%'":'';
            }
        }

        if($arrPost['vigenciainicio'] != null ) {
            if($dataVigenciaInicio = DateTime::createFromFormat('d/m/Y',$arrPost['vigenciainicio'])) {
                $dtVigenciaIni = $dataVigenciaInicio->format('Y-m-d');
                $where .= " and dotdatafimvigencia     >= '{$dtVigenciaIni}' ";
            }
        }

        if($arrPost['vigenciafim'] != null ) {
            if ($dataVigenciaFim = DateTime::createFromFormat('d/m/Y', $arrPost['vigenciafim'])) {
                $dtVigenciaFim = $dataVigenciaFim->format('Y-m-d');
                $where .= " and dotdatafimvigencia     <= '{$dtVigenciaFim}' ";
            }
        }

        if($arrPost['iniid']) {
            $where .= " and ini.iniid = {$arrPost['iniid']} ";
        }

        if($arrPost['mdoex_oficio']) {
            $mdoex_oficio = $arrPost['mdoex_oficio'];
            $where .= ($mdoex_oficio != 'T'?
                ($mdoex_oficio == 'S'?
                    " and mdoex_oficio = '{$mdoex_oficio}' ":
                    " and (mdoex_oficio = '{$mdoex_oficio}' or mdoex_oficio is null) "
                )
                :''
            );
        }

        if ($arrPost['termo_validado'] == 'S') {
	        $where .= " and p.proid in(select
                                            dt.proid
                                       from par3.documentotermo dt
                                       inner join par3.documentotermovalidacao dtv ON dtv.dotid = dt.dotid
                                       where dotstatus = 'A' ) ";
	    }
	    if ($arrPost['termo_validado'] == 'N') {
	        $where .= " and p.proid not in(
	                        select
                            dt.proid
                            from par3.documentotermo dt
                            inner join par3.documentotermovalidacao dtv ON dtv.dotid = dt.dotid
                            where dotstatus = 'A'
                        )
                        and dt.dotnumero is not null
              ";
	    }
	    
	    if ($arrPost['termorejeitado'] == 'S'){
	        $where .= "
	        and p.proid in(
                select proid from par3.documentotermo where dotrejeitado = 'S' and dotstatus = 'I'
                and proid not in (
                        select valido.proid
                        from par3.documentotermo valido
                        where valido.proid = proid
                        and dotstatus = 'A'
                        and dotrejeitado = 'N' order by 1 desc
                )
            order by 1 desc
	        ) ";
	    }
	    
	    if ($arrPost['termorejeitado'] == 'N'){
	        $where .= " and p.proid  = (select proid from par3.documentotermo where dotstatus = 'A' and dotrejeitado = 'N' and proid = p.proid order by 1 desc limit 1) ";
	    }
	    
	    if ($arrPost['termonota'] == 'S'){
	        $where .= " and p.proid in(select proid from par3.documentotermo inner join par3.execucao_notafiscal using (proid) where dotstatus = 'A' )) ";
	    }
	    
	    if ($arrPost['termonota'] == 'N'){
	        $where .= " and p.proid not in(select proid from par3.documentotermo inner join par3.execucao_notafiscal using (proid) where dotstatus = 'A' ) )";
        }

        if ($arrPost['bloqueio_par'] == 'S') {
	        $where .= " AND p.inuid IN (Select inuid from par3.vm_relatorio_quantitativo_pendencias
									WHERE (obras_par = true
										or cacs = true or pne = true
										or siope = true or habilitacao = true) ) ";
	    }
	    if ($arrPost['bloqueio_par'] == 'N') {
	        $where .= " AND p.inuid IN (Select inuid from par3.vm_relatorio_quantitativo_pendencias
									WHERE obras_par = FALSE
										AND cacs = FALSE AND pne = FALSE
										AND siope = FALSE AND habilitacao = FALSE) ";
	    }
	    if ($arrPost['termo_gerado'] == 'S') {
	        $where .= " AND dt.dotnumero is not null ";
	    }
	    if ($arrPost['termo_gerado'] == 'N') {
	        $where .= " AND dt.dotnumero is null ";
	    }
	    if($_REQUEST['numeroprocesso']) {
	        $_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
	        $_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
	        $_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
	        $where .= " and p.pronumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
	    }
	    
	    $innerTermo = '';
	    if($arrPost['termocontrato'] == 'S' || $arrPost['termocontrato'] == 'N'){
	        $in = '';
	        switch($arrPost['termocontrato']){
	            case 'S': $in = 'in'; break;
	            case 'N': $in = 'not in';break;
	            default:break;
	        }
	        
	        $where .= "
                    and p.proid {$in}(
                        select
                        pro.proid
                        FROM par3.processo pro
                        inner join par3.documentotermo doc ON doc.proid = pro.proid AND doc.dotstatus = 'A'
                        inner join par3.execucao_contrato ec ON ec.proid = pro.proid
                        group by pro.proid
                    )
            ";
	    }
	    
	    if($arrPost['intaid'] ){
	        switch ($arrPost['intaid'] ){
	            case 1:
	                $where .= " and p.intaid = 1 ";
	                break;
	            case 2:
	                $where .= " and p.intaid = 2 ";
	                break;
	            default:
	                $where .='';
	                break;
	        }
	    }
	    
		if( $arrPost['codigo'] ){
		    $whereObra = " AND o.obrid = '{$arrPost['codigo']}' ";
		    $wherePlan = " AND pp.inpid = '{$arrPost['codigo']}' ";
		}
		if ($arrPost['tipo_processo'] == 'O') $where .= " AND obj.intoid = 1";
		if ($arrPost['tipo_processo'] == 'P') $where .= " AND obj.intoid not in (1)";
		
		if ($tipoVisualizacao == 'excel') {
		    ob_clean();
		    
		    ini_set("memory_limit", "2048M");
		    set_time_limit(0);
		    
		    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		    header("Pragma: no-cache");
		    header("Content-type: application/xls; name=lista_de_termos_".date("dmY_h").".xls");
		    header("Content-Disposition: attachment; filename=lista_de_termos_".date("dmY_h").".xls");
		    header("Content-Description: MID Gera excel");
		    
		    if( $arrPost['tipodetalhado'] == 'sim' ){
		        $arrayCabecalho = array(
		            'Planejamento',
                    'Ano',
                    'Número',
                    'Processo',
                    'UF',
                    'Entidade',
                    'Tipo de Objeto',
                    'Usuário',
                    'Documento',
                    'Data Criação',
                    'Data Vigência',
                    'Data Validação',
                    'Saldo da Conta',
                    'Valor Processo',
                    'Valor Termo',
                    'Valor Empenho',
                    'Valor Pagamento',
                    'Assistência',
                    'Obras',
                    'CACS',
                    'PNE',
                    'SIOPE',
                    'Habilita',
                    'Contas',
                    'ID (PAR3)',
                    'ID (OBRAS2)',
                    'Situação PAR3',
                    'Situação Obras2',
                    'Validação da OS',
                    '% Físico total',
                    '% Executado Instituição',
                    'De Oficio'
                );
		        $sqlO = $this->model->listarExcelDetalhadoObra($where, $whereObra);
		        $sqlP = $this->model->listarExcelDetalhadoPlanejamento($where, $wherePlan);

		        if( $arrPost['tipo_processo'] == 'O' ){
		            $sql = $sqlO;
		        }elseif( $arrPost['tipo_processo'] == 'P' ){
		            $sql = $sqlP;
		        } else {
		            $sql = '('.$sqlO.') union all ('.$sqlP.')';
		        }
            
		        /*if( $arrPost['tipo_processo'] == 'O' ){
		            $arrayCabecalho = array(
		                'Planejamento', 'Ano', 'Número','Processo','UF', 'Entidade', 'Tipo de Objeto', 'Usuário', 'Data Criação', 'Data Vigência',
		                'Valor Processo', 'Valor Termo', 'Valor Empenho', 'Valor Pagamento', 'Assistência', 'Obras', 'CACS', 'PNE', 'SIOPE', 'Habilita', 'Contas', 'ID (PAR3)', 'ID (OBRAS2)', 'Situação PAR3', 'Situação Obras2', 'Validação da OS', '% Físico total', '% Executado Instituição'
		            );
		            $sql = $this->model->listarExcelDetalhadoObra($where, $whereObra);
		        } elseif( $arrPost['tipo_processo'] == 'P' ){
		            $arrayCabecalho = array('Planejamento', 'Ano', 'Número','Processo','UF', 'Entidade', 'Tipo de Objeto', 'Usuário', 'Data Criação', 'Data Vigência',
		                'Valor Processo', 'Valor Termo', 'Valor Empenho', 'Valor Pagamento', 'Assistência', 'Obras', 'CACS', 'PNE', 'SIOPE', 'Habilita', 'Contas'
		            );
                    $sql = $this->model->listarExcelDetalhadoPlanejamento($where, $wherePlan);
		        }*/
		    } else {
		        $arrayCabecalho = array('Número','Processo','UF', 'Entidade', 'Tipo de Objeto','Usuário','Documento','Data Vigência','Data Validação','Saldo da Conta', 'Valor Processo', 'Valor Termo', 'Valor Empenho', 'Valor Pagamento', 'Assistência', 'Obras', 'CACS', 'PNE', 'SIOPE', 'Habilita', 'Contas','De Ofício');
		        $sql = $this->model->listarDocPAR( $where, $whereObra, $wherePlan );
		    }

		    $esconderColunas = array('proid', 'tipo', 'inuid', 'inuid_pendencia', 'id','dotdatainiciovigencia','dotdadafimvigencia','dotdatainclusao');
		    $camposNumericos = ['valor_processo', 'vlrempenho', 'vlrpago', 'dotvalortermo', 'saldo_processo'];
		    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
		    $listagem->esconderColunas($esconderColunas);
		    $listagem->setCabecalho($arrayCabecalho);
		    $listagem->addCallbackDeCampo('pronumeroprocesso', 'formata_numero_processo_sem_html');
		    $listagem->addCallbackDeCampo($camposNumericos, 'formataNumeroMonetarioSemSimbolo');
		    $listagem->setQuery($sql);
		    $listagem->setId('listagemTermos');
		    $listagem->setCampos($arrayCabecalho);
		    $listagem->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
		    header("Content-type: application/xls; name=lista_de_termos_".date("dmY_h").".xls", true);
		    header("Content-Disposition: attachment; filename=lista_de_termos_".date("dmY_h").".xls", true);
		} elseif ($tipoVisualizacao == 'pdf') {
		    $sql = $this->model->listarDocPAR( $where, $whereObra, $wherePlan );
		    
		    $arrayCabecalho = array('Número','Processo','UF', 'Entidade', 'Tipo de Objeto', 'Data Vigência','Saldo da Conta',  'Valor Processo', 'Valor Termo', 'Valor Empenho', 'Valor Pagamento', 'Assistência', 'Bloqueio PAR');
		    $esconderColunas = array('proid', 'tipo', 'inuid', 'id','dotdatainiciovigencia', 'dotdadafimvigencia','dtvdatavalidacao','dotdatainclusao','mdoex_oficio','mdonome');
		    $listagemSimec = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		    $listagemSimec->setCabecalho($arrayCabecalho);
		    $listagemSimec->esconderColunas($esconderColunas);
		    $listagemSimec->setQuery($sql);
		    $listagemSimec->setId('listagemTermos');
		    $listagemSimec->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
		    $listagemSimec->addCallbackDeCampo(['vlrempenho', 'dotvalortermo', 'vlrpago', 'valor_processo', 'saldo_processo'], 'formataNumeroMonetarioSemSimbolo');
		    $listagemSimec->addCallbackDeCampo(['inuid_pendencia'], 'popOverPendenciarsPar');
		    $listagemSimec->setFormFiltros('form-pesquisa-termo');
		    $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
		    $listagemSimec->render();
		} else {
		    $sql = $this->model->listarDocPAR( $where, $whereObra, $wherePlan );

    		$listagemSimec = new Simec_Listagem();
    		$arrayCabecalho = array('Número','Processo','UF', 'Entidade', 'Tipo de Objeto', 'Data Vigência','Saldo da Conta',  'Valor Processo', 'Valor Termo', 'Valor Empenho', 'Valor Pagamento', 'Assistência', '	Bloqueio PAR');
                $esconderColunas = array('tipo', 'inuid', 'id','dotdatainiciovigencia', 'dotdadafimvigencia','dtvdatavalidacao','dotdatainclusao','mdoex_oficio','mdonome');
    		$listagemSimec->setCabecalho($arrayCabecalho);
    		$listagemSimec->esconderColunas($esconderColunas);
    		$listagemSimec->turnOnPesquisator();
    		$listagemSimec->setQuery($sql);
    		$listagemSimec->setId('listagemTermos');
    		$listagemSimec->addAcao('plus', array('func' => 'detalharProcesso', 'extra-params' => array('tipo')));
    		$listagemSimec->addAcao('file', array('func' => 'gerarTermo', 'cor' => 'success', 'extra-params' => array('tipo', 'inuid')));
    		$listagemSimec->addAcao('view', array('func' => 'visualizaTermoTela', 'extra-params' => array('tipo', 'inuid')));
    		$listagemSimec->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
    		$listagemSimec->addCallbackDeCampo(['vlrempenho', 'dotvalortermo', 'vlrpago', 'valor_processo', 'saldo_processo'], 'formataNumeroMonetarioSemSimbolo');
    		$listagemSimec->addCallbackDeCampo(['inuid_pendencia'], 'popOverPendenciarsPar');
    		$listagemSimec->addCallbackDeCampo('tipo_obrjeto','carregaHistoricoObra');
            //$listagemSimec->addCallbackDeCampo('valor_processo','tituloSaldoProcessoTabela');
    		$listagemSimec->setAcaoComoCondicional('file', [['campo' => 'dotnumero', 'valor' => '', 'op' => 'igual']]);
    		$listagemSimec->setAcaoComoCondicional('view', [['campo' => 'dotnumero', 'valor' => '', 'op' => 'diferente']]);
    		$listagemSimec->setFormFiltros('form-pesquisa-termo');
    		//$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    		$listagemSimec->setTamanhoPagina(50);
    		$listagemSimec->setCampos($arrayCabecalho);
    		
    		$listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
		}
	}
	
	public function listarIniciativasProcesso( $proid, $tipo_processo ){
		
		if( $tipo_processo == 'P' ){
			$sql = $this->model->listarIniciativasProcesso( $proid );
			$arrayCabecalho = array('Codigo', 'Iniciativa', 'Ano', 'Programa', 'Tipo de Objeto', 'Valor de Contrapartida', 'Valor Empenho');
			$esconderColunas = array('tipo', 'inuid', 'iniid', 'proid');
		} else {
			$sql = $this->model->listarIniciativasProcessoObras( $proid );
			$arrayCabecalho = array('Obrid', 'Iniciativa', 'Obra', 'Ano', 'Tipo de Objeto', 'Etapa', 'Programa', 'Valor de Contrapartida', 'Valor Empenho');
			$esconderColunas = array('tipo', 'inuid', 'iniid', 'proid', 'inpid');
		}
		
		$listagemSimec = new Simec_Listagem();
		$listagemSimec->setCabecalho($arrayCabecalho);

		$listagemSimec->esconderColunas($esconderColunas);
		$listagemSimec->setQuery($sql);
		$listagemSimec->addCallbackDeCampo(['vlrempenho', 'inecontrapartida'], 'formataNumeroMonetarioSemSimbolo');
		//$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
		$listagemSimec->setTamanhoPagina(50);
		$listagemSimec->setCampos($arrayCabecalho);
		
		$listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
	}
	
	public function carregaDocumentoTermo( $dotid ){
		$sql = "SELECT dt.dotid, dt.proid, dt.mdoid, dt.docid, dt.dotidpai, dt.dotnumero, dt.dotstatus, dt.dotdatainclusao,
       				dt.usucpfinclusao, dt.dotdataalteracao, dt.usucpfalteracao, dt.dotdatainiciovigencia,
       				dt.dotdatafimvigencia, dt.dotvalortermo, md.tpdcod, dt.dotjustificativa_de_oficio, dt.dotdata_de_oficio, dt.dotid
  				FROM par3.documentotermo dt
                	inner join par3.modelodocumento md on md.mdoid = dt.mdoid and md.mdostatus = 'A'
				WHERE dotid = $dotid and dt.dotstatus in ('A', 'B')";
		$arrDados = $this->model->pegaLinha($sql);
		return $arrDados;
	}
	
	public function excluirDocumento( $dotid )
	{
		$sql = "UPDATE par3.documentotermo SET dotstatus = 'I', dotusucpfexcluxao = '{$_SESSION['usucpf']}', dotdataexclusao = now()  WHERE dotid = $dotid";
		$this->model->executar($sql);
		
		$url = 'par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A';
		
		if( $this->model->commit() ){
			simec_redirecionar($url, 'success', 'Documento Excluído com Sucesso');
		} else {
			simec_redirecionar($url, 'error', 'Erro ao Excluir o Modelo');
		}
		return true;
	}
	
	/*public function gerarTermoDeOficio( $arrPost = array() )
	{
	    $arrPost['aditivo'] = 'S';
	    $arrPost['bloqueado'] = 'S';
	    $arrPost['requisicao'] = 'carregar-termo';
	    if( $arrPost['tipo'] == 'par' ) $arrPost['tipo_processo'] = 'P';
	    else $arrPost['tipo_processo'] = 'O';
	    
	    $arDados = $this->model->pegaLinha("SELECT iu.itrid, 
                                	(SELECT count(ppcid) FROM par3.processoparcomposicao WHERE proid = d.proid AND ppcstatus = 'A') AS tipo_par,
                                	(SELECT count(pocid) FROM par3.processoobracomposicao WHERE proid = d.proid AND pocstatus = 'A') AS tipo_obra
                                FROM par3.processo d
                                	INNER JOIN par3.instrumentounidade iu ON iu.inuid = d.inuid AND iu.inustatus = 'A'
                                WHERE d.proid = {$arrPost['proid']}");
	    
	    if( $arDados['itrid'] == 1 ){
	        $arrPost['mdoid'] = $this->model->pegaUm("SELECT mdoid FROM par3.modelodocumento WHERE mdostatus = 'A' AND mdoex_oficio = 'S' AND mdoesfera = 'E' AND mdotipo = '{$arrPost['tipo_processo']}'");
	    } else {
	        $arrPost['mdoid'] = $this->model->pegaUm("SELECT mdoid FROM par3.modelodocumento WHERE mdostatus = 'A' AND mdoex_oficio = 'S' AND mdoesfera = 'M' AND mdotipo = '{$arrPost['tipo_processo']}'");
	    }
	    $arrPost['dotid_ref'] = $this->model->pegaUm("SELECT max(d.dotid) FROM par3.documentotermo d WHERE d.dotstatus IN ('A', 'B') AND d.proid = {$arrPost['proid']}");
	    
	    $dotid = $this->salvar_termo($arrPost);
	    $arrPost['dotid'] = $dotid;
	    $arrPost['requisicao'] = '';
	    $mdoconteudo = $this->alteraMacroDocumento($arrPost);
	    $arrPost['mdoconteudo_t'] = $mdoconteudo;
	    return $this->salvar_termo($arrPost);
	}*/
	
	public function alteraMacroDocumento( $arrPost = array() )
	{
		
		$sql = "SELECT mdoconteudo FROM par3.modelodocumento WHERE mdostatus = 'A' AND mdoid = ".$arrPost['mdoid'];
		$mdoconteudo = $this->model->pegaUm($sql);
		
		$sql = "SELECT teccampo, tectabela, teccoluna, tecschema, tecdescricao, tecsql
	            FROM par3.termocampos
	            WHERE tecstatus = 'A'";
		
		$arrDados = $this->model->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		foreach ($arrDados as $key => $v) {
		    
			if(strpos($mdoconteudo, $v["teccampo"])) {
				$varAlteracaoMacro = '';
				if( !empty($v['tectabela']) ){
					$sqlMacro = $this->montaSQLMacro($v['tectabela'], $v['teccampo'], $v['teccoluna'], $arrPost['proid']);
					$varAlteracaoMacro = $this->model->pegaUm($sqlMacro);
				} else {
					if( $v['tecsql'] ){
						$sqlMacro = $v['tecsql'];
						$sqlMacro.= " WHERE pro.proid = ".$arrPost['proid'];
						$varAlteracaoMacro = $this->model->pegaUm($sqlMacro);
					} else {
						if( strtolower($v['teccampo']) == strtolower('#Sequencial_Termo#') ){
							$dotnumero = $this->model->pegaUm("select dotnumero||'-'||intoid from par3.documentotermo where proid = {$arrPost['proid']} and dotstatus in ('A', 'B') order by dotid asc LIMIT 1");
							$varAlteracaoMacro = $dotnumero;
						}
						if( strpos( strtolower($v['teccampo']), strtolower('Concedente') ) ){
							if( !empty($v['teccoluna']) ){
								$sql = "SELECT DISTINCT ".$v['teccoluna']." FROM entidade.entidade ent
											INNER JOIN inner join par3.endereco ed on ed.endid = ine.endid ende ON ende.entid = ent.entid
											INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod
										WHERE ent.entstatus = 'A' AND ent.entnumcpfcnpj = '".CPF_PRESIDENTE_FNDE."'";
						
								$varAlteracaoMacro = $this->model->pegaUm($sql);
							}
						}
						if( strtolower($v['teccampo']) == strtolower('#cronograma_inicial#') ) {
						    $varAlteracaoMacro = $this->model->pegaUm("select to_char(now(), 'MM/YYYY')");
						    
						}elseif( strtolower($v['teccampo']) == strtolower('#cronograma_final#') ) {
						    $arCronograma = $this->model->getDadosCronograma($arrPost['proid']);
						    $varAlteracaoMacro = $arCronograma['cronogramafinal'];
						}
						if( strtolower($v['teccampo']) == strtolower('#Tabela_Extrato_execucao_plano_acoes_articuladas_municipios#') ){
							$varAlteracaoMacro = $this->montaTabelaTermoCompromisso($arrPost);
						}
						if( strtolower($v['teccampo']) == strtolower('#Tabela_Extrato_execucao_plano_acoes_articuladas_estados#') ){
							$varAlteracaoMacro = $this->montaTabelaTermoCompromisso($arrPost);
						}
						if( strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Obras_Municipios#') ){
							$varAlteracaoMacro = $this->montaTabelaTermoCompromissoObras($arrPost);
						}
						if( strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Obras_estados#') ){
							$varAlteracaoMacro = $this->montaTabelaTermoCompromissoObras($arrPost);
						}
						if( strtolower($v['teccampo']) == strtolower('#Tabela_Obra_Ampliacao_Reforma#') ){
							$varAlteracaoMacro = $this->montaTabelaTermoCompromissoObras($arrPost);
						}
						if( strpos( strtolower($v["teccampo"]), strtolower('Emenda_Parlamentar') )  ){
						    $sql = "SELECT e.emecod || '/' || e.emeano AS emenda
                                    FROM par3.iniciativa_emenda ie
                                    	JOIN emenda.emenda e ON e.emeid = ie.emeid
                                        JOIN par3.analise a1 ON a1.edeid = ie.edeid AND a1.inpid = ie.inpid AND a1.anaano = e.emeano AND a1.anastatus = 'A'::bpchar
                                     	JOIN par3.processoparcomposicao pp_1 ON pp_1.anaid = a1.anaid AND pp_1.inpid = a1.inpid AND pp_1.ppcstatus::text = 'A'::text
                                    WHERE ie.inestatus = 'A'::bpchar
                                    	AND pp_1.proid = {$arrPost['proid']}";
						    
						    $varAlteracaoMacro = $this->model->carregarColuna($sql);
						    $varAlteracaoMacro = implode(', ', $varAlteracaoMacro);
						}
					}
				}
				if( strtolower($v['teccampo']) == strtolower('#Valor_Empenho#') ) {
				    $vlr_Total_Empenho = $this->pegaValorTotalEmpenhoProcesso($arrPost['proid']);
				    $varAlteracaoMacro = $vlr_Total_Empenho;
				}
				if( strtolower($v['teccampo']) == strtolower('#valor_total_termo#') ) {
				    if($arrPost['tipo_processo'] == 'O'){
				        $sql = "SELECT sum(o.obrvalor) FROM par3.termocomposicao tc
                                	INNER JOIN par3.obra o ON o.obrid = tc.obrid AND o.obrstatus = 'A'
                                	INNER JOIN par3.documentotermo dt ON dt.dotid  = tc.dotid AND dt.dotstatus IN ('A', 'B')
                                WHERE dt.proid = {$arrPost['proid']}
				                    AND dt.dotdatainclusao = (SELECT max(dotdatainclusao) FROM par3.documentotermo WHERE dotstatus IN ('A', 'B') AND proid = dt.proid)";
				        $varAlteracaoMacro = $this->model->pegaUm($sql);
				        $varAlteracaoMacro = ($varAlteracaoMacro ? $varAlteracaoMacro : '0,00');
				    } else {
				        $arDadosItemComposicao = $this->model->getDadosItemComposicao($arrPost);
				        $arDadosItemComposicao = $arDadosItemComposicao ? $arDadosItemComposicao : array();
				        
				        $totalTOT = 0;
				        foreach ($arDadosItemComposicao as $key => $valor) {
				            if( $valor['ipiquantidadeaprovada'] > 0 && $valor['valorreferencia'] > 0 ){
				                $totalTOT += (float)$valor['total'];
				            }
				        }
    				    $varAlteracaoMacro = $totalTOT;
    				    $varAlteracaoMacro = ($varAlteracaoMacro ? $varAlteracaoMacro : '0,00');
				    }
				}
				if( (strtolower($v['teccampo']) == strtolower('#valor_suplementar_entidade#')) || (strtolower($v["teccampo"]) == strtolower('#valor_contrapartida#')) ) {
				    if($arrPost['tipo_processo'] == 'O'){
				        $sql = "SELECT sum(o.obrvalor_contrapartida) FROM par3.termocomposicao tc
                                	INNER JOIN par3.obra o ON o.obrid = tc.obrid AND o.obrstatus = 'A'
                                	INNER JOIN par3.documentotermo dt ON dt.dotid  = tc.dotid AND dt.dotstatus IN ('A', 'B')
                                WHERE dt.proid = {$arrPost['proid']}
				                    AND dt.dotdatainclusao = (SELECT max(dotdatainclusao) FROM par3.documentotermo WHERE dotstatus IN ('A', 'B') AND proid = dt.proid)";
				        $varAlteracaoMacro = $this->model->pegaUm($sql);
				        $varAlteracaoMacro = ($varAlteracaoMacro ? $varAlteracaoMacro : '0,00');
				    } else {
				        				        
				        $contrapartida = $this->pegaValorContrapartidaProcesso( $arrPost['proid'] );
				        $contrapartida = ($contrapartida ? $contrapartida : 0);
				        
				        if( (float)$contrapartida > (int)0 ){
				            $varAlteracaoMacro = $contrapartida;
				        } else {
				            $arDadosItemComposicao = $this->model->getDadosItemComposicao($arrPost);
				            $arDadosItemComposicao = $arDadosItemComposicao ? $arDadosItemComposicao : array();
				            
				            $varTotalTermo = 0;
				            foreach ($arDadosItemComposicao as $key => $valor) {
				                if( $valor['ipiquantidadeaprovada'] > 0 && $valor['valorreferencia'] > 0 ){
				                    $varTotalTermo += (float)$valor['total'];
				                }
				            }
				            
				            $vlr_Total_Empenho = $this->pegaValorTotalEmpenhoProcesso($arrPost['proid']);
				            if( (float)$vlr_Total_Empenho < ((float)$varTotalTermo ) ){
				                $varAlteracaoMacro = ((float)$varTotalTermo  - (float)$vlr_Total_Empenho);
				            }
				        }
				    }
				}
				if( strtolower($v['teccampo']) == strtolower('#valor_fnde#') ) {
				    /*if($arrPost['tipo_processo'] == 'O'){
				        $sql = "SELECT sum(o.obrvalor) - sum(coalesce(o.obrvalor_contrapartida,0)) FROM par3.termocomposicao tc
                                	INNER JOIN par3.obra o ON o.obrid = tc.obrid AND o.obrstatus = 'A'
                                	INNER JOIN par3.documentotermo dt ON dt.dotid  = tc.dotid AND dt.dotstatus IN ('A', 'B')
                                WHERE dt.proid = {$arrPost['proid']}";
				        $varAlteracaoMacro = $this->model->pegaUm($sql);
				        $varAlteracaoMacro = ($varAlteracaoMacro ? $varAlteracaoMacro : '0,00');
				    } else {
				        $sql = "SELECT sum(ai.aicqtdaprovado * ai.aicvaloraprovado) AS total FROM par3.termocomposicao tc
                                	INNER JOIN par3.analise a ON a.anaid = tc.anaid AND a.inpid = tc.inpid AND a.anastatus = 'A'
                                	INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.aicstatus = 'A'
                                	INNER JOIN par3.documentotermo dt ON dt.dotid  = tc.dotid AND dt.dotstatus IN ('A', 'B')
                                WHERE dt.proid = {$arrPost['proid']}";
				        $varTotalTermo = $this->model->pegaUm($sql);
				        
				        $vlr_Total_Empenho = $this->pegaValorTotalEmpenhoProcesso($arrPost['proid']);
				        $varContra = 0;
				        if( (float)$vlr_Total_Empenho < ((float)$varTotalTermo ) ){
				            $varContra = ((float)$varTotalTermo  - (float)$vlr_Total_Empenho);
				        }
				        $varAlteracaoMacro = (float)$varTotalTermo - (float)$varContra;
				        $varAlteracaoMacro = ($varAlteracaoMacro ? $varAlteracaoMacro : '0,00');
				    }*/
				    $varAlteracaoMacro = $this->pegaValorTotalEmpenhoProcesso($arrPost['proid']);
				}
				if( strpos( strtolower($v["teccampo"]), 'valor') || strpos( strtolower($v["teccampo"]), 'soma') ){
				    
				    if( strpos( strtolower($v["teccampo"]), strtolower('Extenso') ) )
				        $varAlteracaoMacro = valorMonetarioExtenso($varAlteracaoMacro);
				        else
				            $varAlteracaoMacro = simec_number_format( $varAlteracaoMacro, 2, ',', '.' );
				}
				$mdoconteudo = str_replace($v["teccampo"], strtoupper($varAlteracaoMacro), $mdoconteudo);
			}
		}
		
		return $mdoconteudo;
	}
	
	public function montaSQLMacro($mictabela, $miccampo, $miccoluna, $proid){
	    
		switch($mictabela) {
			case 'unidade_executora':
			    if( strpos( strtolower($miccampo), strtolower('Nome_do_Gestor_Entidade_Executora') ) || strpos( strtolower($miccampo), strtolower('CPF_do_Gestor_Entidade_Executora') ) ){
				    $join = "INNER JOIN par3.unidade_executora exe on exe.inuid = pro.inuid and exe.ungstatus = 'A' 
                        	INNER JOIN par3.usuarioresponsabilidade ur ON ur.ungid = exe.ungid AND ur.rpustatus = 'A'
                        	INNER JOIN seguranca.usuario usue ON usue.usucpf = ur.usucpf";
				    $alias = "usue";
			    } else {
				    $join = "INNER JOIN par3.unidade_executora exe on exe.inuid = pro.inuid and exe.ungstatus = 'A'";
				    $alias = "exe";
			    }
			break;	
			case 'entidade':
			    $join = "inner join par3.instrumentounidade iu on iu.inuid = pro.inuid
                         inner join par3.instrumentounidade_entidade ine on ine.inuid = pro.inuid and ine.entstatus = 'A' and (case when iu.itrid = 2 then ine.tenid = 1 else ine.tenid = 3 end) and ine.entnome is not null";
				$alias = "ine";
			case 'documentotermo':
				$join = "inner join par3.documentotermo dot on dot.proid = pro.proid and dot.dotid = (select max(dotid) from par3.documentotermo where proid = pro.proid and dotstatus in ('A', 'B'))";
				$alias = "dot";
			break;	
			case 'endereco':
				if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
					$join = "inner join par3.instrumentounidade_entidade ine on ine.inuid = pro.inuid and ine.entstatus = 'A'
    						 inner join par3.instrumentounidade iu on iu.inuid = ine.inuid and iu.inustatus = 'A' and (case when iu.itrid = 2 then ine.tenid = 2 else ine.tenid = 9 end)
						     inner join par3.endereco ed on ed.endid = ine.endid";
					$alias = "ed";
				}else{
					$join = "inner join par3.instrumentounidade iu on iu.inuid = pro.inuid
	                         inner join par3.instrumentounidade_entidade ine on ine.inuid = pro.inuid and ine.entstatus = 'A' and (case when iu.itrid = 2 then ine.tenid = 1 else ine.tenid = 3 end) and ine.entnome is not null
						     inner join par3.endereco ed on ed.endid = ine.endid";
					$alias = "ed";
				}
				break;	
			case 'municipio':
				if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
					$join =	"INNER JOIN par.instrumentounidade iu ON iu.muncod = pro.muncod
							 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO."
							 INNER JOIN territorios.municipio mun on mun.muncod = iu.muncod ";
				}else{
					$join =	"inner join par3.instrumentounidade iu on iu.inuid = pro.inuid and iu.inustatus = 'A'
    							inner join territorios.municipio mun on mun.muncod = iu.muncod ";
				}
				$alias = "mun";
				break;				
			case 'estado':
				$join =	"inner join par3.instrumentounidade iu on iu.inuid = pro.inuid and iu.inustatus = 'A'
                         inner join territorios.estado est on est.estuf = iu.estuf ";
				$alias = "est";
				break;				
			case 'instrumentounidade':
			    $join = "inner join par3.instrumentounidade iu on iu.inuid = pro.inuid and iu.inustatus = 'A'";
			    $alias = "iu";
			break;
			case 'instrumentounidade_entidade':
				if( strpos( strtolower($miccampo), strtolower('Entidade') ) ){
					$join = "inner join par3.instrumentounidade iu on iu.inuid = pro.inuid
	                         inner join par3.instrumentounidade_entidade ine on ine.inuid = pro.inuid and ine.entstatus = 'A' and (case when iu.itrid = 2 then ine.tenid = 1 else ine.tenid = 3 end) and ine.entnome is not null";
					$alias = "ine";
				}
				if( strpos( strtolower($miccampo), strtolower('Concedente') ) ){
					$join = "inner join par3.instrumentounidade_entidade ine on ine.inuid = pro.inuid and ine.entstatus = 'A' and ine.entcpf = '".CPF_PRESIDENTE_FNDE."'";
					$alias = "ine";
				}
				if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
					$join = "inner join par3.instrumentounidade_entidade ine on ine.inuid = pro.inuid and ine.entstatus = 'A' and ine.tenid = 2";
					$alias = "ine";
				}
				if( strpos( strtolower($miccampo), strtolower('Secretario') ) ){
					$join = "inner join par3.instrumentounidade_entidade ine on ine.inuid = pro.inuid and ine.entstatus = 'A' and ine.tenid = 9";
					$alias = "ine";
				}
				if( strpos( strtolower($miccampo), strtolower('Secretaria') ) ){
					$join = "inner join par3.instrumentounidade_entidade ine on ine.inuid = pro.inuid and ine.entstatus = 'A' and ine.tenid = 3";
					$alias = "ine";
				}
			break;
			case 'empenho':
				$join = "inner join par3.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso and emp.empstatus = 'A' and emp.empcodigoespecie = '01'";
				$alias = "emp";
			break;
			case 'obra':
				$join = "INNER JOIN par3.processoobracomposicao po ON po.proid = pro.proid AND pocstatus = 'A'
                    	 INNER JOIN(
                    		SELECT DISTINCT o.inpid, o.obrid, o.obrdsc, o.obrcep, o.obrlogradouro, o.obrnumero, o.obrcomplemento, o.obrbairro,
                            	o.estuf, o.obrvalor, o.obrvalor_contrapartida, (o.obrvalor - coalesce(o.obrvalor_contrapartida,0)) AS obrvalor_fnde, ot.otpdsc, esc.escnome, esc.esccodinep
                            FROM par3.obra o
                            	INNER JOIN par3.obra_tipo ot ON ot.otpid = o.otpid
                            	LEFT JOIN(
                            		select DISTINCT esc.escid, esc.escnome, obrque.obrid, esc.esccodinep from par3.escola as esc
                                    inner join par3.obra_x_questionario as obrque on obrque.escid = esc.escid
                                    where esc.escco_esfera in('M','E')
                            	) esc ON esc.obrid = o.obrid
                            WHERE o.obrstatus = 'A'
                    	 ) obr ON obr.obrid = po.obrid";
				$alias = "obr";
			break;
			case 'iniciativa_obra':
				$join = "INNER JOIN par3.processoobracomposicao po ON po.proid = pro.proid AND pocstatus = 'A'
                    	INNER JOIN par3.obra o ON o.obrid = po.obrid AND o.obrstatus = 'A'
                    	INNER JOIN(
                    		SELECT i.iniid, i.iniid||' - '||ind.inddsc as inddsc, ip.inpid FROM par3.iniciativa i
                                INNER JOIN par3.iniciativa_planejamento ip ON ip.iniid = i.iniid AND ip.inpstatus = 'A'
                    			INNER JOIN par3.iniciativa_descricao ind ON ind.indid = i.indid AND ind.indstatus = 'A'
                    		WHERE i.inistatus = 'A'
                    	) ini ON ini.inpid = o.inpid";
				$alias = "ini";
			break;
		}

		if( !empty($alias) ) $alias = $alias.'.';

		$miccolunaR = $alias.$miccoluna;
		if( in_array($miccoluna, array('entcpf', 'entcnpj', 'ungcnpj', 'usucpf') ) ) $miccolunaR = ' formata_cpf_cnpj('.$alias.$miccoluna.') ';
		if( in_array($miccoluna, array('dotdatafimvigencia') ) ) $miccolunaR = " to_char(".$alias.$miccoluna.", 'DD/MM/YYYY') ";
		if( in_array($miccoluna, array('pronumeroprocesso') ) ) $miccolunaR = ' formata_num_processo('.$alias.$miccoluna.') ';

		$sql 	= 	"SELECT ".$distinct." ".$miccolunaR." FROM par3.processo pro ";
		$sql 	.= 	$join;
		$sql 	.= 	" WHERE pro.prostatus = 'A' and pro.proid = ".$proid.";";
		
		return $sql;
	}

	public function montaTabelaTermoCompromisso( $arrPost ){

		$proid = $arrPost['proid'];
		$this->countHtml = 1;

		$obProcesso = new Par3_Controller_Processo();
		$arrEntidade = $obProcesso->carregaDadosEntidadeProcesso($arrPost['proid']);
		$arTipoObjeto = $obProcesso->pegaTipoObjetoPorProcesso($arrPost['proid']);

		$arrPost['esfera'] = $arrEntidade['esfera'];

		if( $arrEntidade['esfera'] == 'E' ){
			$arrPost['tenid'] = array(3, 9);
		} else {
			$arrPost['tenid'] = array(1, 2);
		}

		$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= $this->htmlIdentificacaoEnteFederado($arrPost);
		if( in_array($arTipoObjeto['intoid'], $this->arrTipoObjetoTermo) ){
			$tabela .= $this->htmlIdentificacaoAcoesFinanciadas_Formacao_Evento($arrPost);
		} else {
			$tabela .= $this->htmlIdentificacaoAcoesFinanciadas($arrPost);
		}
		$tabela .= $this->htmlDadosEmpenho($arrPost);
		$tabela .= $this->htmlCronogramaExecucaoFisico($arrPost);
		$tabela .= '
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).'  ETAPAS OU FASES (SE HOUVER)</b></td>
				</tr>
				</table>';
		$tabela .= $retorno['tabela'];
		$total = $totalEmp+$retorno['total'];

		$tabela .= '(*) Item a ser adquirido por adesão à ata de registro de preços do FNDE<br><br>';

		if( in_array($arrPost['tpdcod'], array(102, 21)) ) $this->enviaEmailTermoGerado($arrPost);

		return $tabela;
	}

	public function montaTabelaTermoCompromissoObras( $arrPost ){

		$proid = $arrPost['proid'];

		$this->countHtml = 1;

		$obProcesso = new Par3_Controller_Processo();
		$arrEntidade = $obProcesso->carregaDadosEntidadeProcesso($arrPost['proid']);

		$arrPost['esfera'] = $arrEntidade['esfera'];

		if( $arrEntidade['esfera'] == 'E' ){
			$arrPost['tenid'] = array(3, 9);
		} else {
			$arrPost['tenid'] = array(1, 2);
		}

		$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= $this->htmlIdentificacaoEnteFederado($arrPost);
		$tabela .= $this->htmlIdentificacaoAcoesFinanciadasObras($arrPost);
		$tabela .= $this->htmlCronogramaExecucaoFisico($arrPost);
		$tabela .= '</table><br><br>';

		if( in_array($arrPost['tpdcod'], array(102, 21)) ) $this->enviaEmailTermoGerado($arrPost);

		return $tabela;
	}

	public function htmlIdentificacaoEnteFederado( $arrPost ){
		$proid = $arrPost['proid'];

		$arDadosProcesso = $this->model->getDadosProcesso($proid);
		$arDadosEntidade = $this->model->getDadosPrefeitoPrefeitura($arDadosProcesso['inuid'], $arrPost['tenid'][0], $arrPost['tenid'][1]);
		$arEntidadeExecutora = $this->model->getDadosEntidadeExecutora($proid, $arDadosProcesso['inuid']);

		if($arrPost['esfera'] == 'E'){
			$labelEntidade = 'NOME DA SECRETARIA DE EDUCAÇÃO DO ESTADO';
			$labelIdentificacao = 'IDENTIFICAÇÃO DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO';
			$labelUf = 'ESTADO';
		} else {
			$labelEntidade = 'NOME DA PREFEITURA';
			$labelIdentificacao = 'IDENTIFICAÇÃO DO(A) PREFEITO(A)';
			$labelUf = 'MUNICÍPIO';
		}

		$exerc = substr($arDadosProcesso['pronumeroprocesso'], 11, 4);
		$arDadosProcesso['pronumeroprocesso'] = substr($arDadosProcesso['pronumeroprocesso'], 0, 5) . '.' . substr($arDadosProcesso['pronumeroprocesso'], 5, 6) . '/' . substr($arDadosProcesso['pronumeroprocesso'], 11, 4) . '-' . substr($arDadosProcesso['pronumeroprocesso'], 15, 2);
		$tabela = '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO<br>PLANO DE AÇÕES ARTICULADAS  PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['pronumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - '.$labelEntidade.'</b><br>'.$arDadosEntidade['entnome'].'</td>
						<td><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arDadosEntidade['entcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arDadosEntidade['endlogradouro'].' '.$arDadosEntidade['endnumero'].' - '.$arDadosEntidade['endbairro'].'</td>
						<td><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - '.$labelUf.'</b><br>'.$arDadosEntidade['mundescricao'].'</td>
						<td><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arDadosEntidade['estuf'].'</td>
					</tr>';
		 if( $arEntidadeExecutora['ungcnpj'] ){
		 $tabela .= '<tr>
						<td colspan="2"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - NOME DA EXECUTORA</b><br>'.$arEntidadeExecutora['ungrazao_social'].'</td>
						<td><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arEntidadeExecutora['ungcnpj']).'</td>
					</tr>
                    <tr>
						<td><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO DA EXECUTORA</b><br>'.$arEntidadeExecutora['endlogradouro'].' '.$arEntidadeExecutora['endnumero'].' - '.$arEntidadeExecutora['endbairro'].'</td>
						<td><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - '.$labelUf.' DA EXECUTORA</b><br>'.$arEntidadeExecutora['mundescricao'].'</td>
						<td><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arEntidadeExecutora['estuf'].'</td>
					</tr>';
		 }
		 $tabela .= '<tr style="text-align: left;">
						<td colspan="3"><b>'.$labelIdentificacao.'</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arDadosEntidade['prefeito'].'</td>
						<td valign="top"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arDadosEntidade['cpfprefeito']).'</td>
					</tr>';
		 if( $arEntidadeExecutora['ungcnpj'] ){
		 $tabela .= '<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - NOME DO GESTOR(A) da ENTIDADE EXECUTORA</b><br>'.$arEntidadeExecutora['usunome'].'</td>
						<td valign="top"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arEntidadeExecutora['usucpf']).'</td>
					</tr>';
		 }
		return $tabela;
	}

	public function htmlIdentificacaoAcoesFinanciadas( $arrPost ){
		$proid = $arrPost['proid'];
		
		$arDadosItemComposicao = $this->model->getDadosItemComposicao($arrPost);

		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;

		$tabela .= '<tr>
						<td colspan="3" valign="top">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%" colspan="8"><b>Itens</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="40%"><b>Item</b></td>
									<td width="15%"><b>Etapa</b></td>
									<td width="20%"><b>Unidade</b></td>
									<td width="10%"><b>Ano</b></td>
									<td width="10%"><b>Quantidade</b></td>
									<td width="10%"><b>Preço Unitário</b></td>
									<td width="10%"><b>Total</b></td>
								</tr>';

		foreach ($arDadosItemComposicao as $key => $valor) {

			if( $valor['ipiquantidadeaprovada'] > 0 && $valor['valorreferencia'] > 0 ){

				$totalQTD += (float)$valor['ipiquantidadeaprovada'];
				$totalVLR += (float)$valor['valorreferencia'];
				$totalTOT += (float)$valor['total'];
				$tabela .= '	<tr>
								<td>'.$valor['itcdsc'].'</td>
								<td>'.$valor['etadsc'].'</td>
								<td>'.$valor['unidsc'].'</td>
								<td>'.$valor['iniano'].'</td>';
				$tabela .= '<td style="text-align: center;">'.$valor['ipiquantidadeaprovada'].'</td>
								<td style="text-align: right;">R$ '.($valor['valorreferencia'] ? simec_number_format($valor['valorreferencia'],2,",",".") : '').'</td>
								<td style="text-align: right;"><b>R$ '.($valor['total'] ? simec_number_format($valor['total'],2,",",".") : '').'</b></td>
							</tr>';
			}
		}

		$contrapartida = $this->pegaValorContrapartidaProcesso($proid);
		$contrapartida = ($contrapartida ? $contrapartida : 0);

		if( (float)$contrapartida < (int)1 ){
    		$vlr_Total_Empenho = $this->pegaValorTotalEmpenhoProcesso($proid);

    		if( (float)$vlr_Total_Empenho < ((float)$totalTOT) ){
    		    $contrapartida = ((float)$totalTOT - (float)$vlr_Total_Empenho);
    		}
		}

		//$_SESSION['par3']['termo']['dotvalortermo'] = $totalTOT;
		if( $arrPost['contrapartida_reformulacao'] > 0 ) $contrapartida = $arrPost['contrapartida_reformulacao'];
		
		if( $arrPost['dotid'] )
		  $this->model->executar("update par3.documentotermo set dotvalortermo = {$totalTOT}, dotvalorcontrapartida = {$contrapartida} where dotid = {$arrPost['dotid']}");

		$tabela .= '
							<tr>
								<td align="center" colspan="4"><b>Total Geral</b></td>
								<td style="text-align: center;">'.$totalQTD.'</td>
								<td style="text-align: right;">R$ '.simec_number_format( ($totalVLR),2,",",".").'</td>
								<td style="text-align: right;"><b><div id="div_valor_termo">R$ '.simec_number_format( ($totalTOT),2,",",".").'</div></b></td>
							</tr>';
		if( (float)$contrapartida > (int)0 ){
		    $tabela .= '
							<tr>
								<td align="center" colspan="7"><b>Total de Contrapartida:</b> <b>R$ '.simec_number_format($contrapartida,2,",",".").'</b></td>
							</tr>';
		}
		$tabela .= '	</table>
					</td>
				</tr>';
		return $tabela;
	}

	public function htmlIdentificacaoAcoesFinanciadasObras( $arrPost ){
		$proid = $arrPost['proid'];

		$arDadosItemComposicao	= $this->model->getDadosObrasComposicao($arrPost);

		$tabela .= 	'
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="60%"><b>Ações(Nome da Obra)</b></td>
									<td width="15%"><b>Etapa</b></td>
									<td width="30%"><b>Tipo Obra</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="15%"><b>Valor(R$)</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;

		$arrLocalizacao = array();

		foreach ($arDadosItemComposicao as $key => $valor) {

				$localizacao = 'Bairro: '.$valor['obrbairro'].', '.'Logradouro: '.$valor['obrlogradouro'].', '.'Cidade: '.$valor['mundescricao'].'.<br>';
				$arrLocalizacao[] = array(
										'obras' => $valor['obra'],
										'localizacao' => $localizacao
									);

				$totalTOT += $valor['vlrobra'];
				$tabela .= '	<tr>
									<td>'.$valor['obra'].'</td>
									<td>'.$valor['etadsc'].'</td>
									<td>'.$valor['otpdsc'].'</td>
									<td style="text-align: center;">1</td>
									<td style="text-align: right;">R$ '.($valor['vlrobra'] ? simec_number_format($valor['vlrobra'],2,",",".") : '').'</td>
								</tr>';
		}
		//$_SESSION['par3']['termo']['dotvalortermo'] = $totalTOT;
		if( $arrPost['dotid'] )
		    $this->model->executar("update par3.documentotermo set dotvalortermo = {$totalTOT} where dotid = {$arrPost['dotid']}");
		
		$tabela .= '
						<tr>
							<td align="center" colspan=4><b>Total Geral</b></td>
							<td style="text-align: right;"><b><div id="div_valor_termo">R$ '.simec_number_format( ($totalTOT),2,",",".").'</div></b></td>
						</tr>
					</table>
				</td>
			</tr>
			<tr style="text-align: left;">
				<td colspan="3"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).' - LOCALIZAÇÃO</b><br>
					<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
						<tr align="left" valign="top">
							<td><b>Nome da Obra</b></td>
							<td><b>Endereço</b></td>
						</tr>';
		foreach ($arrLocalizacao as $ende) {
			$tabela.= '<tr align="left" valign="top">
							<td>'.$ende['obras'].'</td>
							<td>'.$ende['localizacao'].'</td>
						</tr>';
		}
		return $tabela;
	}

	public function htmlIdentificacaoAcoesFinanciadas_Formacao_Evento( $arrPost ){
		$proid = $arrPost['proid'];
		$arDadosItemComposicao = $this->model->getDadosItemComposicaoGrupoIniciativa($arrPost);

		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;

		$tabela .= '<tr>
						<td colspan="3" valign="top">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%" colspan="6"><b>Itens</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="05%"><b>Código</b></td>
									<td width="65%"><b>Iniciativa</b></td>
									<td width="65%"><b>Etapa</b></td>
									<td width="10%"><b>Total</b></td>
								</tr>';

		foreach ($arDadosItemComposicao as $key => $valor) {

				$totalQTD += (float)$valor['ipiquantidadeaprovada'];
				$totalVLR += (float)$valor['valorreferencia'];
				$totalTOT += (float)$valor['total'];
				$tabela .= '	<tr>
								<td>'.$valor['iniid'].'</td>
								<td>'.$valor['inddsc'].'</td>
								<td>'.$valor['etadsc'].'</td>
								<td style="text-align: right;"><b>R$ '.($valor['total'] ? simec_number_format($valor['total'],2,",",".") : '').'</b></td>
							</tr>';
		}

		$contrapartida = $this->pegaValorContrapartidaProcesso($proid);
		$contrapartida = ($contrapartida ? $contrapartida : 0);

		if( (float)$contrapartida < (int)1 ){
		    $vlr_Total_Empenho = $this->pegaValorTotalEmpenhoProcesso($proid);

		    if( (float)$vlr_Total_Empenho < ((float)$totalTOT) ){
		        $contrapartida = ((float)$totalTOT - (float)$vlr_Total_Empenho);
		    }
		}

		//$_SESSION['par3']['termo']['dotvalortermo'] = ($totalTOT - $contrapartida);
		if( $arrPost['contrapartida_reformulacao'] > 0 ) $contrapartida = $arrPost['contrapartida_reformulacao'];
		
		if( $arrPost['dotid'] ){
		    $this->model->executar("update par3.documentotermo set dotvalortermo = ".($totalTOT - $contrapartida).", dotvalorcontrapartida = {$contrapartida} where dotid = {$arrPost['dotid']}");
		    $this->model->commit();
		}

		$tabela .= '
							<tr>
								<td align="center" colspan="3"><b>Total Geral</b></td>
								<td style="text-align: right;"><b><div id="div_valor_termo">R$ '.simec_number_format( ($totalTOT),2,",",".").'</div></b></td>
							</tr>';
		if( (float)$contrapartida > (int)0 ){
		    $tabela .= '
							<tr>
								<td align="center" colspan="7"><b>Total de Contrapartida:</b> <b>R$ '.simec_number_format($contrapartida,2,",",".").'</b></td>
							</tr>';
		}

		$tabela .= '	</table>
					</td>
				</tr>';
		return $tabela;
	}

	public function htmlDadosEmpenho( $arrPost ){
		$proid = $arrPost['proid'];
		$arrEmpenho = $this->model->getDadosEmpenho($proid);

		$tabela .= '
			<tr style="text-align: left;">
				<td colspan="3">
					<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
						<tr style="text-align: center;">
							<td colspan="3"><b>EMPENHOS</b></td>
						</tr>
						<tr align="center" valign="top">
							<td width="50%"><b>Iniciativa</b></td>
							<td width="25%"><b>Número</b></td>
							<td width="25%"><b>Valor</b></td>
						</tr>';
		$totalEmp = 0;
		foreach ($arrEmpenho as $v) {
			$totalEmp += (float)$v['valor'];
			$tabela.='<tr valign="top">
						<td align="left">'.$v['iniciativa'].'</td>
						<td align="center">'.$v['empnumero'].'</td>
						<td align="right">R$ '.simec_number_format($v['valor'], 2, ",", ".").'</td>
					</tr>';
		}

		$tabela.= '<tr>
						<td align="center" colspan=2><b>Total Empenho</b></td>
						<td style="text-align: right;"><b>R$ '.simec_number_format($totalEmp,2,",",".").'</b></td>
					</tr>
				</table>
			</td>
		</tr>';

		return $tabela;
	}

	public function htmlCronogramaExecucaoFisico( $arrPost ){
		$proid = $arrPost['proid'];
		
		if( $arrPost['reformulacao'] == 'O' || $arrPost['reformulacao'] == 'T' ){
		    $arCronograma['cronogramainicial'] = $arrPost['dotdatainiciovigencia'];
		    $arCronograma['cronogramafinal'] = $arrPost['dotdatafimvigencia'];
		} else {
		    $arCronograma = $this->model->getDadosCronograma($proid);
		}
		$tabela .= '
			<tr style="text-align: left;">
				<td colspan="3"><b>'.str_pad($this->countHtml++, 2, 0, STR_PAD_LEFT).'  CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
			</tr>
			<tr style="text-align: left;" valign="top">
				<td colspan="3">
					<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
						<tr style="text-align: left;" valign="top">
							<td width="50%"><b>Data inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
							<td width="50%"><b>Data final:</b><br>'.($arrPost['dotdata_de_oficio'] ? $arrPost['dotdata_de_oficio'] : $arCronograma['cronogramafinal']).'</td>
						</tr>
					</table>
				</td>
			</tr>';

		return $tabela;
	}

	public function gravaHtmlDocumento($doptexto, $idDocumento, $processo, $tipo){
		ob_clean();

		//$doptexto = str_replace("\'", '"', $doptexto);

		if( $doptexto ){
			if( strpos($doptexto, '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
				$doptexto = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
			} else {
				$doptexto = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
			}
		}
		$doptexto = $doptexto ? simec_htmlspecialchars($doptexto) : 'null';

		$nomeArquivo 		= 'Minuta_Documento_'.$tipo.'_'.$idDocumento.'_'.$processo.'_'.date('YmdHis');
		$diretorio		 	= APPRAIZ . 'arquivos/par3/documentoTermo';
		$diretorioArquivo 	= APPRAIZ . 'arquivos/par3/documentoTermo/'.$nomeArquivo.'.txt';

		if( !is_dir($diretorio) ){
			mkdir($diretorio, 0777);
		}

		$fp = fopen($diretorioArquivo, "w");
		if ($fp) {
			stream_set_write_buffer($fp, 0);
			fwrite($fp, $doptexto);
			fclose($fp);
		}

		$this->model->executar("update par3.documentotermoarquivo set dtastatus = 'I' where dotid = $idDocumento");

		$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
				VALUES( '".$nomeArquivo."',
						'txt',
						'".$nomeArquivo."',
						'text/plain',
						'".filesize($diretorioArquivo)."',
						'".date('Y-m-d')."',
						'".date('H:i:s')."',
						'".$_SESSION["usucpf"]."',
						".($_SESSION['sisid'] ? $_SESSION['sisid'] : 'null').",
						'A') RETURNING arqid";
		$arqid = $this->model->pegaUm($sql);

		$sql = "insert into par3.documentotermoarquivo(dotid, arqid, proid, dtatipo, dtanomearquivo, dtastatus)
				values($idDocumento, $arqid, $processo, '$tipo', '".$nomeArquivo.'.txt'."', 'A')";
		$this->model->executar($sql);
		$this->model->commit();
		return true;
	}

	public function pegaAssinaturaTermo( $dotid, $html ){

		$dadosValidacao = $this->carregaValidacaoTermo( $dotid );

		if( !empty($dadosValidacao[0]['usucpf']) ){
			$textoValidacao = "";
			if(is_array($dadosValidacao)){
				foreach( $dadosValidacao as $dv ){
					$textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
				}
			}

			$html = trim(html_entity_decode( $html ))."
			<table id=termo align=center border=0 cellpadding=3 cellspacing=1 >
			<tr style=\"text-align: center;\">
			<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>$textoValidacao</td>
			</tr>
			</table>";
			$html = str_replace("'", '', $html);
		}

		return $html;
	}

	public function pegaTermoCompromissoArquivo( $dotid, $boMinuta = false  ){

		$dtanomearquivo = $this->model->pegaUm("select d.dtanomearquivo from par3.documentotermoarquivo d where d.dotid = {$dotid} order by d.dtaid desc");

		if( $dtanomearquivo ){
			$diretorio = APPRAIZ . 'arquivos/par3/documentoTermo/'.$dtanomearquivo;
			if( is_file($diretorio) ){
				$doptexto = file_get_contents($diretorio);

				if( $boMinuta == false ){
					$doptexto = $this->pegaAssinaturaTermo($dotid, $doptexto);
				}
			}
		}

		return simec_html_entity_decode($doptexto);
	}

	public function listaTermoPorProcesso( $arrPost = array() ){
		$proid = $arrPost['dados'][0];
		$tipo_processo = $arrPost['dados'][1];

		$sql = $this->model->carregarTermoPorProcesso( $proid );
		$arrDados = $this->model->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrayCabecalho = array('Número', 'Documento', 'Vigência', 'Valor', 'Situação', 'Incluído por', 'Alterado por', 'Status', 'Reformulação');

		$arrPerfil = pegaPerfils($_SESSION['usucpf']);

		echo getToobarListagem($sql, false);
		?>
		<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="3">
		<thead>
			<tr>
				<th width="180px;">&nbsp;</th>
<?php 			foreach ($arrayCabecalho as $cab) {
					echo '<th class="">'.$cab.'</th>';
				}?>
			</tr>
		</thead>
		<tbody>
<?php 	if( $arrDados ){?>
			<tr>
<?php		foreach ($arrDados as $v) {
    
    /*esdid_doc,
    md.mdoex_oficio,*/

                if( $v['mdoex_oficio'] == 'S' ){
                    $acaoEditarTermo 	= '<a href="javascript:abrirModalTermoOficio('.$v['dotid'].', \'\', '.$v['proid'].', '.$v['inuid'].', \''.$tipo_processo.'\');" title="De Ofício"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a> ';
                } else {
                    $acaoEditarTermo 	= '<a href="javascript:carregarTermo('.$v['dotid'].', '.$v['proid'].');" title="Editar"><span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span></a> ';
                }
                
				$acaoVisualizaTermo = '<a href="javascript:visualizaTermo('.$v['dotid'].');" title="Abrir"><span class="btn btn-primary btn-sm glyphicon glyphicon-eye-open"></span></a> ';
				$acaoDeletaTermo 	= '<a href="javascript:deletarTermo('.$v['dotid'].', \'N\', \''.$v['vlrpago'].'\');" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a> ';
				//$acaoRegerarTermo 	= '<a href="javascript:deletarTermo('.$v['dotid'].', \'N\', \''.$v['vlrpago'].'\');" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-refresh"></span></a> ';
				$acaoSigarp 		= '<span class="btn btn-default btn-sm glyphicon glyphicon-repeat"></span>';

				//<span class="btn btn-info btn-sm glyphicon glyphicon-refresh"></span>
				if( $v['dotrejeitado'] == 'S' ){
				    $v['situacao'] = 'Documento Rejeitado Contrapartida';
				}
				#2447 - Prorrogação De Ofício Aprovada
				if( !in_array($v['dotstatus'], array('A', 'B') ) || $v['status'] == 'Excluído' || ($v['situacao'] == 'Documento Validado' /*|| $v['esdid_doc'] == PAR3_ESDID_OFICIO_APROVADA*/ ) ){
				    $acaoDeletaTermo = '<span class="btn btn-default btn-sm glyphicon glyphicon-trash"></span> ';
					$acaoEditarTermo = '<span class="btn btn-default btn-sm glyphicon glyphicon-pencil"></span> ';
				}
				
				if( $v['mdoex_oficio'] == 'S' && $v['esdid_doc'] == PAR3_ESDID_OFICIO_APROVADA && !in_array(PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) ){
				    $acaoDeletaTermo = '<span class="btn btn-default btn-sm glyphicon glyphicon-trash"></span> ';
				}
				
				$sql = "select count(dt.dotid) 
                        from par3.documentotermo dt
                        	inner join par3.modelodocumento md on md.mdoid = dt.mdoid and md.mdostatus = 'A'
                        	inner join workflow.documento d on d.docid = dt.docid
                        where dt.dotstatus in ('A', 'B')
                        	and dt.proid = $proid
                        	and md.mdoex_oficio = 'S'
                        	and d.esdid in (".PAR3_ESDID_OFICIO_CADASTRAMENTO.", ".PAR3_ESDID_OFICIO_AGUARDANDO_APROVACAO_COORDENADOR.")";
				$boExOficio = $this->model->pegaUm($sql);
				
				$acaoEX = '';
				if( in_array($v['dotstatus'], array('A', 'B')) && ($v['situacao'] == 'Documento Validado' || $v['mdoex_oficio'] == 'S') && $arrDados['dias'] >= 0 && (int)$boExOficio == (int)0 ){
				    $acaoEX = '&nbsp;<a href="javascript:abrirModalTermoOficio(\'\', '.$v['dotid'].', '.$v['proid'].', '.$v['inuid'].', \''.$tipo_processo.'\');" title="De Ofício"><span class="btn btn-success btn-sm">
                                    <i class="fa fa-file-text-o"></i>
                                </span></a>';
				}
				/*
				2445 - Em cadastramento
				2446 - Aguardando Aprovação do Coordenador
				2447 - Prorrogação De Ofício Aprovada
				2448 - Prorrogação De Ofício Cancelada
				*/
				if( in_array(PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) &&
				        ($v['situacao'] == 'Documento Validado' || $v['situacao'] == 'Documento Rejeitado Contrapartida') &&
				        in_array($v['dotstatus'], array('A', 'B') ) ){
				    $acaoDeletaTermo 	= '<a href="javascript:deletarTermo('.$v['dotid'].', \'S\', \''.$v['vlrpago'].'\');" title="Apagar"><span class="btn btn-danger btn-sm glyphicon glyphicon-trash"></span></a> ';
				}
				// || in_array($v['esdid'], array(2446, 2447, 2448) )
				if( $v['mdoex_oficio'] == 'S' ){
				    //$acaoDeletaTermo = '<span class="btn btn-default btn-sm glyphicon glyphicon-trash"></span> ';
				}

				if( ($v['situacao'] == 'Documento Validado' || ($v['mdoex_oficio'] == 'S' && $v['esdid_doc'] == PAR3_ESDID_OFICIO_APROVADA) ) && $v['status'] != 'Excluído' && $v['dotstatus'] == 'A' && $v['intoid'] != 1  ) #intoid = 1 -> Obra
					$acaoSigarp		 = '<a href="javascript:visualizarSigarp('.$v['dotid'].', '.$v['proid'].');" title="Dados SIGARP"><span class="btn btn-warning btn-sm glyphicon glyphicon-repeat"></span></a> ';

				echo '
				<tr>
					<td class="text-center" style="width: 210px">'.$acaoEditarTermo.$acaoVisualizaTermo.$acaoDeletaTermo.$acaoSigarp.$acaoEX.'</td>
					<td>'.$v['dotnumero'].'</td>
					<td>'.$v['termo'].'</td>
					<td>'.$v['dotdatafimvigencia'].'</td>
					<td>R$ '.simec_number_format($v['dotvalortermo'], 2, ',', '.').'</td>
					<td>'.$v['situacao'].'</td>
					<td>'.$v['inclusao'].'</td>
					<td>'.$v['alterou'].'</td>
					<td>'.$v['status'].'</td>
					<td>'.($v['reformulacao'] ? $v['reformulacao'] : '-').'</td>
				</tr>';
			}
		} else {?>
			<tr class="lista-vazia">
				<td colspan="14">
					<div class="alert alert-info col-lg-8 col-lg-offset-2" role="alert">Nenhum registro encontrado</div>
				</td>
			</tr>
<?php }?>
		</tbody>
		</table>
		<?php
	}

	public function visualizaDadosSigarp( $proid, $dotid ){

		$sql = $this->model->sqlVisualizaDadosSigarp($dotid);

		$listagemSimec = new Simec_Listagem();
		$arrayCabecalho = array('Iniciativa', 'Itens', 'Id SIGARP', 'Quantidade', 'Valor', 'Ano', 'Adesão');
		$esconderColunas = array('procnpj', 'pronumeroprocesso', 'dotnumero', 'dotdatafimvigencia', 'inpid', 'itcid');
		$listagemSimec->setCabecalho($arrayCabecalho);
		$listagemSimec->setTitulo('Itens de Composição');
		$listagemSimec->esconderColunas($esconderColunas);
		$listagemSimec->turnOnPesquisator();
		$listagemSimec->setQuery($sql);
		//$listagemSimec->addAcao('edit', array('func' => 'carregarTermo', 'extra-params' => array('itcid')));
		$listagemSimec->addCallbackDeCampo('ipivalorreferencia', 'formataNumeroMonetarioComSimbolo');
		$listagemSimec->setTamanhoPagina(50);
		$listagemSimec->setCampos($arrayCabecalho);
		$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
		$listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);


		echo '<input type="hidden" name="dotid_sigarp" id="dotid_sigarp" value="'.$dotid.'">
			  <input type="hidden" name="proid_sigarp" id="proid_sigarp" value="'.$proid.'">';
		$sql_l = $this->model->sqlVisualizaLogSigarp($dotid);

		$listagemSimec = new Simec_Listagem();
		$arrayCabecalho = array('Serviço', 'Nome', 'Mensagem', 'Data');
		$esconderColunas = array('lgwerro');
		$listagemSimec->setCabecalho($arrayCabecalho);
		$listagemSimec->setTitulo('Log SIGARP');
		$listagemSimec->esconderColunas($esconderColunas);
		$listagemSimec->setQuery($sql_l);
		$listagemSimec->setTamanhoPagina(50);
		$listagemSimec->setCampos($arrayCabecalho);
		$listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
	}

	public function podeReenviarSigarp( $dotid ){

		$sql = $this->model->sqlVisualizaDadosSigarp($dotid, false, true);
		$arrAdesao = $this->model->pegaLinha($sql);

		if( !empty($arrAdesao) ){
		    $total_envio = $this->model->pegaUm("SELECT count(iasid) FROM par3.itemadesaosigarp WHERE dotid = $dotid and adestatus = 'A'");
		    if( (int)$total_envio == (int)0 ){
		        return 'S';
		    } else {
		      return 'A';
		    }
		} else {
    		$sql = $this->model->sqlVisualizaDadosSigarp($dotid, true);
    		$arrDadosSigarp = $this->model->carregar($sql);

    		if( !empty($arrDadosSigarp[0]) ){
    		    return 'G';
    		} else {
    		    $sql = $this->model->sqlVisualizaLogSigarp($dotid);
    		    $arrDados = $this->model->pegaLinha($sql);

        		if( is_array($arrDados) && !empty($arrDados['lgwservico']) && ($arrDados['lgwerro'] == 't') ){
        			return 'S';
        		} else {
        			return 'N';
        		}
    		}
		}
	}

	public function verificaTermoAtivo( $proid, $tipo_processo ){
		$sql = "select count(dotid) from par3.documentotermo where proid = $proid and dotstatus in ('A', 'B')";
		$boTem = $this->model->pegaUm($sql);

		$url = 'par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A&proid='.$proid;

		if( (int)$boTem > (int)0 ){
			simec_redirecionar($url, 'error', 'Não é possivel continuar, pois ja existe um termo ativo para este processo!');
		}

		$sql = "SELECT count(e.empid) FROM par3.processo p
                INNER JOIN par3.empenho e ON e.empnumeroprocesso = p.pronumeroprocesso
                WHERE e.empstatus = 'A' AND e.empcodigoespecie = '01'
                	AND e.empsituacao ILIKE '%efetivado%'
                	AND p.proid = $proid";
		$boTemEmpenho = $this->model->pegaUm($sql);
		if( (int)$boTemEmpenho == (int)0 ){
		    simec_redirecionar($url, 'error', 'Não é possivel continuar, pois este processo não possui empenho efetivado!');
		}

		if( $_SESSION['usucpf'] != '' ){
    		if( $tipo_processo == 'O' ){
        		$sql = "SELECT count(o.obrid) FROM par3.processoobracomposicao pp
                        	INNER JOIN par3.obra o ON o.obrid = pp.obrid AND o.obrstatus = 'A'
                        	INNER JOIN workflow.documento d ON d.docid = o.docid
                        WHERE pp.proid = {$proid}
                        	AND d.esdid = 2050";
        		$boObraAprovada = $this->model->pegaUm($sql);
        		if( (int)$boObraAprovada == (int)0 ){
        		    simec_redirecionar($url, 'error', 'Não é possivel continuar, pois este processo não possui todas as obras aprovadas!');
        		}
    		}
		}
	}

    /**
     * Verifica se existe algum documento pendente para uma unidade
     * @param $inuid
     * @return bool
     */
	public function verificaDocumentosPendentes($inuid){

        $sql = $this->model->montarSqlDocumentosPendentes($inuid);
        $existe = $this->model->pegaUm($sql);

        if($existe){
            return true;
        }

        return false;

    }

    /**
     * Função listaPAR3
     * - monta a lista de documentos PAR.
     *
     *
     */
    public function listaPAR3($arrPost)
    {
        $sql = $this->model->montarSQLPAR3($arrPost);

        $cabecalho = array('Processo', 'Documento', 'Tipo de Documento', 'Situação',
            'Data de Vigência', 'Valor do Termo', 'Valor Empenhado',
            'Pagamento Efetivado', 'Saldo Bancário (CC + CP + Fundo)');

        $listagem = new   Simec_Listagem( );
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas('dotid', 'proid', 'nome_termo', 'intoid','obrid', 'totalobrid', 'dfibanco', 'dfiagencia', 'dficonta');
        $listagem->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
        $listagem->addCallbackDeCampo('termo','formata_Tipo_Documento');
        $listagem->addCallbackDeCampo(Array('dotvalortermo', 'valor_empenhado', 'pagamento_efetivado', 'saldo_bancario'),'formataNumeroMonetarioComSimbolo');
        $listagem->addCallbackDeCampo('saldo_bancario', 'formataSaldoComInformacoes' );
        $listagem->addAcao('plus', ['func' => 'listaHistoricoPAR3', 'titulo' => 'Listar Histórico', 'extra-params' => ['proid']]);
        if( $arrPost['pendente'] > 0 ){
        	$listagem->addAcao('inserir', ['func' => 'visualizarTermoPar3', 'titulo' => 'Validar']);
        }else{
            $listagem->addAcao('gear', ['func' => 'modalReprogramacao', 'titulo' => 'Reprogramação', 'extra-params' => ['proid']]);
            $listagem->addAcao('retornar', ['titulo' => 'Acompanhamento', 'func' => 'detalharAcompanhamento', 'extra-params' => ['proid', 'obrid', 'totalobrid']]);
        }

        $listagem->turnOnOrdenacao();
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaPAR3()

    public function listaDocumentoManifesto($inuid)
    {
        $sql = "SELECT estuf FROM par3.instrumentounidade WHERE inuid = $inuid AND itrid = 1";
        $estuf = $this->model->pegaUm($sql);

        if($estuf){
            $sql = "SELECT dot.domid, to_char(dot.domdatainclusao, 'YYYY')||lpad(cast(dot.domid as varchar),5,'0') AS numero,
                    	'<span class=\"domid\" value=' || dot.domid || '></span>' || mdo.mdonome as termo,
                    	CASE
                    		WHEN dot.domopcao = 1 THEN 'Interesse integral'
                    		WHEN dot.domopcao = 2 THEN 'Interesse parcial'
                    		WHEN dot.domopcao = 3 THEN 'Sem Interesse'
                    	ELSE 'Documento não Validado' END  AS opcao,
                        (SELECT to_char(dtvdatavalidacao, 'DD/MM/YYYY') AS datas FROM par3.documentotermovalidacao WHERE domid = dot.domid AND dtvstatus = 'A') AS datas,
                        (SELECT count(dtvid) FROM par3.documentotermovalidacao WHERE domid = dot.domid AND dtvstatus = 'A') AS validacao
                    FROM
                    	par3.documentotermomanifesto dot
                        INNER JOIN par3.modelodocumento mdo ON dot.mdoid = mdo.mdoid
                    WHERE dot.domstatus = 'A'
                    	AND dot.estuf = '{$estuf}'";

            $cabecalho = array('Nº do Documento', 'Tipo de Documento', 'Situação', 'Data de Aceite');

            $listagem = new   Simec_Listagem( );
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
            $listagem->esconderColunas(array('domid', 'validacao'));
            $listagem->addCallbackDeCampo('termo','formata_Tipo_Documento_Manifesto');
            $listagem->addAcao('inserir', ['func' => 'visualizarTermoManifesto', 'titulo' => 'Validar']);
            $listagem->setAcaoComoCondicional('inserir', [['campo' => 'validacao', 'valor' => '0', 'op' => 'igual']]);
            $listagem->turnOnOrdenacao();
            $listagem->turnOffForm();
            $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        }

    }//end listaPAR3()

    public function listaHistoricoPAR3($arrPost){
        global $db;
        $sql = Par3_Model_DocumentoTermo::montarSQLHistoricoPAR3($arrPost);

        $cabecalho = array('', 'Documento', 'Tipo de Documento', 'Data de Vigência', 'Valor do Termo', 'Reformulação', 'Situação');
        
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addAcao('view', 'visualizarTermoPar3');
        $listagem->addCallbackDeCampo(['dotvalortermo'],'formataNumeroMonetarioComSimbolo');
        //        $listagem->addAcao('view', array('func' => 'vizualizarTermo', 'extra-params' => array('dopid')));
        
        $listagem->turnOffForm();
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

	public function enviaEmailTermoGerado( $arrPost = array() ){

		$numTermo = $this->model->pegaUm( "select d.dotnumero||'-'||d.intoid from par3.documentotermo d where d.proid = {$arrPost['proid']} and d.dotstatus in ('A', 'B')" );

		if( $arrPost['esfera'] == 'E' ){
			$arrPost['tenid'] = array(3, 9);
		} else {
			$arrPost['tenid'] = array(1, 2);
		}
		$arDadosProcesso = $this->model->getDadosProcesso( $arrPost['proid'] );
		$arDadosEntidade = $this->model->getDadosPrefeitoPrefeitura($arDadosProcesso['inuid'], $arrPost['tenid'][0], $arrPost['tenid'][1]);

		$arDadosProcesso['pronumeroprocesso'] = substr($arDadosProcesso['pronumeroprocesso'], 0, 5) . '.' . substr($arDadosProcesso['pronumeroprocesso'], 5, 6) . '/' . substr($arDadosProcesso['pronumeroprocesso'], 11, 4) . '-' . substr($arDadosProcesso['pronumeroprocesso'], 15, 2);

		$conteudo = '<p><span style="font-size: small;">Prezada(o) Dirigente,</span></p>
<p><span style="font-size: small;">Comunicamos que o termo de compromisso n&deg; </span><strong>'.$numTermo.'</strong><span style="font-size: small;">, referente ao processo </span><strong>'.$arDadosProcesso['pronumeroprocesso'].'</strong><span style="font-size: small;">, dessa entidade, se encontra dispon&iacute;vel para aceite. Dessa forma, indicamos acessar a funcionalidade Execu&ccedil;&atilde;o e Acompanhamento do Plano de A&ccedil;&otilde;es Articuladas (PAR 2016/2019), e efetuar a valida&ccedil;&atilde;o do Termo de Compromisso.</span></p>
<p><span style="font-size: small;">Para isso, o(a) <strong>Gestor(a)</strong> deve acessar o SIMEC (<a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a>), utilizando seu <strong>CPF</strong> e <strong>senha</strong> e seguir seguintes passos:</span></p>
<ol>
<li><span style="font-size: small;">Clicar no m&oacute;dulo PAR e selecionar a op&ccedil;&atilde;o: <strong>Execu&ccedil;&atilde;o e Acompanhamento</strong>;</span></li>
<li><span style="font-size: small;">Clicar na aba <strong>Documentos;</strong></span></li>
<li><span style="font-size: small;">Selecionar o Termo, pelo tipo de documento, e clicar em <strong>Aceitar</strong>;</span></li>
</ol>
<p><span style="font-size: small;">Atenciosamente,</span></p>
<p><span style="font-size: small;">Equipe T&eacute;cnica do Plano de A&ccedil;&otilde;es Articuladas - PAR<br /> Fundo Nacional de Desenvolvimento da Educa&ccedil;&atilde;o<br /> Minist&eacute;rio da Educa&ccedil;&atilde;o<br /> Telefones: (61) 2022-5854 / 5382/ 5953/ 5815/5914/5807/4559<br /> PAR Fale Conosco. Link: <span style="text-decoration: underline;"><a href="https://www.fnde.gov.br/parfaleconosco/index.php/publico">https://www.fnde.gov.br/parfaleconosco/index.php/publico</a></span> (Acesso site FNDE), ou pelo Fale Conosco dispon&iacute;vel na aba inferior do m&oacute;dulo PAR (Acesso site SIMEC).</span></p>';

		$assunto = 'MEC/FNDE - Documento(s) para validação (PAR 2016/2019)';

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), 'wesleysilva@mec.gov.br', $assunto, $conteudo, $cc, $cco );
		} else {
			enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $arDadosEntidade['entemail'], $assunto, $conteudo, $cc, $cco );
		}
	}

	public function montaHtmlTermo( $texto = '' ){

		$html = '<html>
						<head>
							<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
							<meta http-equiv="Cache-Control" content="no-cache">
							<meta http-equiv="Pragma" content="no-cache">
							<meta http-equiv="Expires" content="-1">
							<title>Termo de Compromisso</title>
							<style type="text/css" media="print">
								@media print {
									.notprint { display: none }
								 	 html, body {
								        margin: 0;
								        padding: 0;
								        border: 0;
								    }
								    #printable {
								        margin: 0;
								        padding: 0;
								        border: 0;
								        font-size: 14px;
								    }
								    #printable ~ * {
								        display: none;
								    }
								}
							</style>'.(IS_PRODUCAO ? require_once APPRAIZ . 'includes/google_analytics.php' : '').'
						</head>
						<body>
						<div id="topo">
							<table width="100%" style="position:static;">
								<thead>
									<tr>
										<th class="noPadding" align="left">
										<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
											<tr bgcolor="#ffffff">
												<td valign="top" align="center" width="100%"><img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0"></td>
											</tr>
											<tr>
												<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;" bgcolor="#ffffff">
													<b><font style="font-size:11px;font-family:arial;color:#9F9F9F;">MINISTÉRIO DA EDUCAÇÃO<br />FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</font></b>
												</td>
											</tr>
										</table>
										</th>
									</tr>
								</thead>
								<tbody>
									<tr>
										<td width="100%">
										<input type="hidden" name="dotid" id="dotid" value="">
										<input type="hidden" name="domid" id="domid" value="">
											<div id="div_conteudo_termo" style="font-weight:normal;vertical-align:top; FONT: 8pt Arial,verdana; width: 100%">'.$texto.'</div>
										</td>
									</tr>
								</tbody>
							</table>
						</div>
						
					</body>
				</html>';
		return $html;
	}

	public function imprimirTermoPDF( $dotid ){


		include_once APPRAIZ.'includes/classes/RequestHttp.class.inc';
		$mdoconteudo = $this->pegaTermoCompromissoArquivo( $dotid );
		$mdoconteudo = str_ireplace('\"', '"', $mdoconteudo);

		$nome_termo = $this->model->pegaUm("
				SELECT
					md.mdonome||'_'||ito.intodsc as termo
				FROM par3.documentotermo dt
					inner join par3.modelodocumento md on md.mdoid = dt.mdoid and md.mdostatus = 'A'
					inner join par3.iniciativa_tipos_objeto ito on ito.intoid = dt.intoid
				WHERE dt.dotid = {$_REQUEST['dotid']}");

		$mdoconteudo = ($this->montaHtmlTermo( $mdoconteudo ));

		$http = new RequestHttp();
		echo $http->toPdfDownload( $mdoconteudo, $nome_termo );
	}
	
	public function imprimirTermoManifestoPDF( $domid ){

		include_once APPRAIZ.'includes/classes/RequestHttp.class.inc';
		
		$dadosTermo = $this->model->pegaLinha("SELECT dom.domid ,
	    
	    domopcao,
	        dta.dtanomearquivo,
	        dpv.dtvcpf as cpfgestor,
				to_char(dpv.dtvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
                                                           us.usunome
                FROMpar3.documentotermomanifesto dom
                	LEFT JOINpar3.documentotermoarquivo dta ON dta.domid = dom.domid
                    LEFT JOIN par3.documentotermovalidacao dpv ON dpv.domid = dom.domid
                                                    LEFT JOIN seguranca.usuario us ON us.usucpf = dpv.dtvcpf
                WHERE dom.domid = {$domid} ANDdomstatus = 'A'");
                	$html = "";

        if( !empty($dadosTermo['cpfgestor']) ){
            $textoValidacao = "<b>Validado por ".$dadosTermo['usunome']." - CPF: ".formatar_cpf($dadosTermo['cpfgestor'])." em ".$dadosTermo['data']." </b><br>";

            $html = "<br><br><table id=termo align=center border=0 cellpadding=3 cellspacing=1 >
                    <tr style=\"text-align: center;\">
		<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>$textoValidacao</td>
		</tr>
                    </table>";
		$html = str_replace("'", '', $html);
		}
		echo $html;
	}

	public function imprimirModeloTermoPDF( $mdoid ){

		include_once APPRAIZ.'includes/classes/RequestHttp.class.inc';

		$obDocumento = new Par3_Controller_Modelo_Documento();

		$arrDados = $obDocumento->carregaModeloDocumento( $mdoid );

		$http = new RequestHttp();
		echo $http->toPdfDownload( $arrDados['mdoconteudo'], $arrDados['mdonome'] );
	}

	public function salvarAceiteTermo( $dotid ){

		$arProcesso = $this->verificaValidacaoTermo($dotid);

		$url = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid='.$arProcesso['inuid'];

		if( $arProcesso['profinalizado'] ){
			simec_redirecionar($url, 'error', 'Termo não pode ser validado pois o processo já foi finalizado.');
			exit();
		}

		if( $_SESSION['usucpf'] != $_SESSION['usucpforigem'] ){
			simec_redirecionar($url, 'error', 'Ação não permitida para simulação de usuarios.');
			exit();
		}

		if($_SESSION['usucpf'] == '00000000191' ){
			simec_redirecionar($url, 'error', 'O CPF do usuario carregado na sessão é inválido!');
			exit();
		}

		$sql = "INSERT INTO par3.documentotermovalidacao ( dotid, dtvcpf, dtvdatavalidacao, dtvstatus )
				VALUES ( ".$dotid.", '".$_SESSION['usucpf']."', NOW(), 'A' )";
		$this->model->executar( $sql );
		$this->model->commit();
		
		//WS_USUARIO_SIGEF, WS_SENHA_SIGEF
		$obSigarp = new Par3_Controller_WS_Sigarp_FNDE();
		$obSigarp->dotid 		= $dotid;
		$obSigarp->ws_usuario 	= WS_USUARIO_SIGEF;
		$obSigarp->ws_senha 	= WS_SENHA_SIGEF;
		
		$arReformulado = $this->verificaTermoTemReformulacao($arProcesso['proid']);
		
		if( $arReformulado['refid'] ){
		    # 5654 - Finalizar reprogramação
		    require_once APPRAIZ . 'includes/workflow.php';
		    
		    $sql = "SELECT DISTINCT rd.refid, d.dotid, rd.tirid, rd.docid, ac.aedid 
                    FROM par3.documentotermo d
                    	INNER JOIN par3.reformulacao_documento rd ON rd.refid = d.refid
                    	INNER JOIN workflow.documento dc ON dc.docid = rd.docid
                    	INNER JOIN workflow.acaoestadodoc ac ON ac.esdidorigem = dc.esdid AND ac.esdiddestino in (2371, 2462)
                    WHERE d.proid = {$arProcesso['proid']} AND rd.refidpai IS NOT NULL
                    	AND dc.esdid IN (2375, 2461)";
		    $arrWorkRef = $this->model->carregar($sql);
		    $arrWorkRef = $arrWorkRef ? $arrWorkRef : array();
		    
		    foreach ($arrWorkRef as $v) {
		        wf_alterarEstado( $v['docid'], $v['aedid'], 'Fluxo de Reformulação', array('refid' => $v['refid'], 'cache_estado_atual' => 0) );
		        if( $v['tirid'] == 2 ){
		            $obSigarp->dotid = $v['dotid'];
		            $sigarp = $obSigarp->enviarItensSigarpReformulacao();
		        }
		    }
		} else {
		    if( $arProcesso['intoid'] != 1 ){
		        $boTem = $this->model->pegaUm("SELECT par3.retornar_termo_pendente_validacao($dotid)");
		        if( empty($boTem) ){
                    $sigarp = $obSigarp->enviarItensSigarp();
		        }
		    } else {
		        $arrObra = $this->model->carregarColuna("SELECT obrid FROM par3.termocomposicao WHERE dotid = $dotid AND tecstatus = 'A'");
		        $arrObra = $arrObra ? $arrObra : array();
		        
		        foreach ($arrObra as $obrid) {
		            $this->model->executar("select par3.importarobraparaobras2($obrid);");
		        }
		        $this->model->commit();
		    }
		}

		simec_redirecionar($url, 'success', 'O aceite foi confirmado com sucesso!');
		exit();
	}
	
	public function verificaTermoTemReformulacao( $proid ){
	    $sql = "SELECT dt1.dotid, rd.refid, rd.docid, rd.tirid FROM par3.documentotermo dt1
					INNER JOIN par3.reformulacao_documento rd ON rd.refid = dt1.refid
					INNER JOIN workflow.documento d ON d.docid = rd.docid
				WHERE dt1.proid = $proid AND d.esdid in (".PAR3_REFORMULACAO_PRAZO_ESDID_EM_VALIDACAO_GESTOR.", ".PAR3_REFORMULACAO_ESDID_AGUARDANDO_VALIDACAO_GESTOR.") 
                    AND rd.refstatus = 'A' AND dt1.dotstatus in ('A') order by rd.refid desc";
	    return $this->model->pegaLinha($sql);
	}

	public function verificaValidacaoTermo( $dotid ){

		$arProcesso = $this->model->pegaLinha("select distinct md.mdonome||'_'||ito.intodsc as termo, dp.mdoid, p.proid, p.profinalizado, p.inuid, dp.dotid, dp.intoid,
                                                    md.mdoqtdvalidacao as qtd_validar,
                                                    (select count(d.dtvid) from par3.documentotermovalidacao d where d.dotid = dp.dotid and d.dtvstatus = 'A'
                                                    	AND dtvcpf NOT IN (	
															SELECT usucpf FROM par3.usuarioresponsabilidade WHERE rpustatus = 'A' AND ungid IS NOT NULL AND pflcod = 1862
														)) as qtd_validado,
                                                    (SELECT count(ur.rpuid) FROM par3.documentotermovalidacao dt
                                    					INNER JOIN par3.usuarioresponsabilidade ur ON ur.usucpf = dt.dtvcpf AND ur.rpustatus = 'A'
                                    					INNER JOIN par3.unidade_executora ue ON ue.ungid = ur.ungid AND ue.ungstatus = 'A'
                                    				WHERE dt.dotid = dp.dotid AND dtvstatus = 'A') AS qtd_validado_executora,
                                                    COALESCE((SELECT count(ue.ungcnpj)
                                                		FROM par3.unidade_executora ue
                                                			INNER JOIN par3.processo po ON po.inuid = ue.inuid AND po.procnpj = ue.ungcnpj AND po.prostatus = 'A'
                                                		WHERE ue.inuid = p.inuid AND po.proid = p.proid AND ue.ungcnpj = p.procnpj),0) AS qtd_validar_executora
                                                from par3.documentotermo dp
                                                    inner join par3.modelodocumento md on md.mdoid = dp.mdoid and md.tpdcod in (21, 102, 103) and md.mdostatus = 'A'
                                                    inner join par3.iniciativa_tipos_objeto ito on ito.intoid = dp.intoid
                                                    inner join par3.processo p on p.proid = dp.proid
                                                where
                                                    dp.dotid = $dotid and dp.dotstatus = 'A'");
		return $arProcesso;
	}

	public function carregaValidacaoTermo( $dotid ){
		$sql = "SELECT
				    dpv.dtvcpf as cpfgestor,
				    to_char(dpv.dtvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
				    us.usunome,
				    us.usucpf
				FROM par3.documentotermo dp
				    inner join par3.documentotermovalidacao dpv ON dpv.dotid = dp.dotid
				    inner join seguranca.usuario us ON us.usucpf = dpv.dtvcpf
				WHERE
				    dpv.dtvstatus = 'A' and
				    dp.dotid = ".$dotid;

		return $this->model->carregar($sql);
	}

	public function pegaValorContrapartidaProcesso( $proid ){
	    $sql = "SELECT
                    coalesce(sum(ie.inecontrapartida),0)::numeric(20,2) AS inecontrapartida
                FROM par3.iniciativa_emenda ie
                	JOIN emenda.emenda e ON e.emeid = ie.emeid
                    JOIN par3.analise a1 ON a1.edeid = ie.edeid AND a1.inpid = ie.inpid AND a1.anaano = e.emeano AND a1.anastatus = 'A'::bpchar
                 	JOIN par3.processoparcomposicao pp_1 ON pp_1.anaid = a1.anaid AND pp_1.inpid = a1.inpid AND pp_1.ppcstatus::text = 'A'::text
                WHERE ie.inestatus = 'A'::bpchar
                	AND pp_1.proid = $proid";
	    return $this->model->pegaUm($sql);
	}

	public function pegaValorTotalEmpenhoProcesso( $proid ){
	    $sql = "SELECT vlrempenho FROM par3.v_saldo_empenho_do_processo WHERE proid = $proid";
	    return $this->model->pegaUm($sql);
	}

	public function carregarTermoManifestacao( $arrPost = array() ){

	    $sql = "SELECT iue.entid, iue.entnome, iue.entcpf, iue.entemail, iue.entcnpj, iue.entrazaosocial
                FROM par3.instrumentounidade iu
                	INNER JOIN par3.instrumentounidade_entidade iue ON iue.inuid = iu.inuid AND iue.entstatus = 'A'
                WHERE estuf = '{$arrPost['estuf']}' AND itrid in (1,3)
                	AND iu.inustatus = 'A'
                	AND tenid = 3";
	    $arSecretaria = $this->model->pegaLinha($sql);

	    $sql = "SELECT iue.entid, iue.entnome, formata_cpf_cnpj(iue.entcpf) as entcpf, iue.entemail, iue.entcnpj, iue.entrazaosocial
                FROM par3.instrumentounidade iu
                	INNER JOIN par3.instrumentounidade_entidade iue ON iue.inuid = iu.inuid AND iue.entstatus = 'A'
                WHERE estuf = '{$arrPost['estuf']}' AND itrid in (1,3)
                	AND iu.inustatus = 'A'
                	AND tenid = 9";
	    $arSecretario = $this->model->pegaLinha($sql);

	    $sql = "SELECT mdoconteudo FROM par3.modelodocumento WHERE mdoid = 16";
	    $mdoconteudo = $this->model->pegaUm($sql);

	    /*
	     #CPF_do_Secretario_Educacao#
	     #UF_Dirigente#
	     #Dia_Atual#/#Hora_Atual#
	     #Nome_do_Secretario_Educacao# / #CPF_do_Secretario_Educacao#
	     * */
	    $mdoconteudo = str_replace('#Nome_do_Secretario_Educacao#', strtoupper($arSecretario['entnome']), $mdoconteudo);
	    $mdoconteudo = str_replace('#UF_Dirigente#', strtoupper($arrPost['estuf']), $mdoconteudo);
	    $mdoconteudo = str_replace('#CPF_do_Secretario_Educacao#', strtoupper($arSecretario['entcpf']), $mdoconteudo);
	    $mdoconteudo = str_replace('#Dia_Atual#', strtoupper(date('d')), $mdoconteudo);
	    $mdoconteudo = str_replace('#Hora_Atual#', strtoupper(date("H:i:s")), $mdoconteudo);

	    return $mdoconteudo;
	}

	public function recuperaTermoManifestacao($inuid){
	    $sql = "SELECT dom.domid,
                       domopcao,
                       dta.dtanomearquivo,
                       dpv.dtvcpf as cpfgestor,
                       to_char(dpv.dtvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
                       us.usunome
                FROM par3.documentotermomanifesto dom
                LEFT JOIN par3.documentotermoarquivo dta ON dta.domid = dom.domid
                LEFT JOIN par3.documentotermovalidacao dpv ON dpv.domid = dom.domid
                LEFT JOIN seguranca.usuario us ON us.usucpf = dpv.dtvcpf
                INNER JOIN par3.instrumentounidade inu ON inu.estuf = dom.estuf
                WHERE inu.inuid = {$inuid} AND domstatus = 'A'";
	    return $this->model->pegaLinha($sql);
    }
    
    public function permissaoAceiteTermo( $dotid, $inuid ){
        
        $arrDocumento = $this->carregaDocumentoTermo($dotid);
        
        if( !empty($arrDocumento['dotdata_de_oficio']) ){
            return false;
        } else {        
            $arrPerfil = pegaArrayPerfil($_SESSION['usucpf']);
            
            $arrPerfilExecutora = array(PAR3_PERFIL_ENTIDADE_EXECUTORA);
            $arrPerfilEntidade = array(PAR3_PERFIL_SECRETARIO_ESTADUAL, PAR3_PERFIL_PREFEITO);
            $arrPerfilPodeTudo = array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR);
                    
            $tem_Resp = $this->verificaResponsabilidadeTermo($inuid);
            
            //if( (int)$tem_Resp > (int)0 || (checaPerfilValido($arrPerfilEntidade, $arrPerfil) == 'S') ){
            $tem_Validacao_Executora = 'N';
            $tem_Validacao_Entidade = 'N';
            $arProcesso = $this->verificaValidacaoTermo($dotid);
            #VERIFICA SE EXISTE ALGUMA VALIDAÇÃO DE ENTIDADE EXECUTORA PARA O TERMO
            if( (int)$arProcesso['qtd_validar_executora'] > (int)0 ){
                if( $arProcesso['qtd_validado_executora'] < $arProcesso['qtd_validar_executora'] ){
                    $tem_Validacao_Executora = 'S';
                }
            }
            #VERIFICA SE EXISTE ALGUMA VALIDAÇÃO DE ENTIDADE PARA O TERMO
            if ($arProcesso['qtd_validado'] < $arProcesso['qtd_validar']) {
                $tem_Validacao_Entidade = 'S';
            }
            #VERIFICA SE O USUÁRIO LOGADO TEM PERMISSÃO DE VALIDAR O TERMO
            $habilitaAceite = false;
            if( (checaPerfilValido($arrPerfilPodeTudo, $arrPerfil) == 'S') && ($tem_Validacao_Executora == 'S' || $tem_Validacao_Entidade == 'S') ){
                $habilitaAceite = true;
            } elseif((checaPerfilValido($arrPerfilExecutora, $arrPerfil) == 'S') && $tem_Resp == 'S' && $tem_Validacao_Executora == 'S'){
                $habilitaAceite = true;
            } elseif( (checaPerfilValido($arrPerfilEntidade, $arrPerfil) == 'S') && $tem_Resp == 'S'&& $tem_Validacao_Entidade == 'S' ){
                $habilitaAceite = true;
            } else {
                $habilitaAceite = false;
            }
            /*} else {
                $habilitaAceite = false;
            }*/
            //ver($arProcesso, $tem_Resp, (int)$habilitaAceite,d);
            return $habilitaAceite;
        }
    }
    
    public function pegaIdWorkFlow($dotid = '', $esdid = '2445')
    {
        require_once APPRAIZ . '/includes/workflow.php';
        
        if ($refid) {
            $arDados = $this->carregaDocumentoTermo($dotid);
            $docid = $arDados['docid'];
        } else {
            // descrição do documento
            $docdsc = "Fluxo de Prorrogação de Termo - De Ofício - dotid: " . $dotid;
            // cria documento do WORKFLOW
            $docid = wf_cadastrarDocumento(398, $docdsc, $esdid);
        }
        return $docid;
    }
    
    public function montarTelaTermoExOficio($dotid_filho = '', $dotid_original, $inuid, $proid, $tipo)
    {
        require_once APPRAIZ . '/includes/workflow.php';
        
        global $simec;
        $obDocumento = new Par3_Controller_DocumentoTermo();
        
        $obProcesso = new Par3_Controller_Processo();
        $arrEntidade = $obProcesso->carregaDadosEntidadeProcesso($proid);
        
        $controleUnidade = new Par3_Controller_InstrumentoUnidade();
        
        $habilita = 'S';
        if( $dotid_filho ){
            $obDocumento = new Par3_Controller_DocumentoTermo();
            $arrDocumentoTermo = $obDocumento->carregaDocumentoTermo($dotid_filho);
            $mdoconteudo = $obDocumento->pegaTermoCompromissoArquivo($dotid_filho, true);
            
            $mdoid = $arrDocumentoTermo['mdoid'];
            $dotdata_de_oficio = $arrDocumentoTermo['dotdata_de_oficio'];
            $dotjustificativa_de_oficio = $arrDocumentoTermo['dotjustificativa_de_oficio'];
            
            $esdid = $this->model->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = {$arrDocumentoTermo['docid']};");
            
            if( !in_array($esdid, array(PAR3_ESDID_OFICIO_CADASTRAMENTO)) ){
                $habilita = 'N';
            }
        }
        //ver($dotid_filho, $dotid_original, $arrDocumentoTermo,d);
        if( $tipo == 'obra' ){
            $mdotipo = 'O';
        } else {
            $mdotipo = 'P';
        }
        ?>
        <div class="ibox">    
        	<div class="row">
                <div class="col-md-12">
                    <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($inuid); ?></h5>
                </div>
            </div>
            
                <div class="ibox-content">
                    <div class="row">
                        <div class="col-md-12 orcamento">
                            <?php
                           /* if ( $arrEntidade['intaid'] == 2) {
                                $obEmenda = new Par3_Controller_EmendasParlamentares();
                                //$edeid, $inpid = '', $inuid = ''
                                echo $obEmenda->pegaBeneficiarioSelecionado('', $inp->inpid, $inuid);
                            }*/ ?>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-12 orcamento">
                            <?php
                                $obProcesso = new Par3_Controller_Processo();
                                echo $obProcesso->carregaDadosOrcamentario($inuid, '', '', $proid);
                            ?>
                        </div>
                    </div>
                </div>
			<div class="row">
                <div class="col-md-12">
                    <h2 class="center">Gerar Termos</h2>
                </div>
            </div>
            <div class="ibox-content">
                <div class="row">
                    <form name="form-gerar-termo" id="form-gerar-termo" class="form-horizontal" method="post">
                        <input type="hidden" name="requisicao" id="requisicao" value="">
                        <input type="hidden" name="proid" id="proid" value="<?php echo $proid; ?>">
                        <input type="hidden" name="tipo" id="tipo" value="<?php echo $tipo; ?>">
                        <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid; ?>">
                        <input type="hidden" name="dotid_original" id="dotid_original" value="<?php echo $dotid_original; ?>">
                        <input type="hidden" name="dotid" id="dotid" value="<?php echo $dotid_filho; ?>">
                        <input type="hidden" name="habilita" id="habilita" value="<?php echo $habilita; ?>">
                        <input type="hidden" name="mdoconteudo_t" id="mdoconteudo_t" value="<?=simec_htmlentities($mdoconteudo); ?>" />
                        <div id="div_mdoconteudo" style="display: none"><?=($mdoconteudo); ?></div>
				<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
            	<tr>
            		<td style="width: 90%">
                        <div class="form-group ">
                            <label for="" class="col-sm-2 col-md-2 col-lg-2 control-label">Tipo de Documento: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <select name="tpdcod" id="tpdcod" maxlength="255" readonly="readonly" class="form-control" data-placeholder="Selecione">
            						<option value="21" label="Termo Aditivo" selected="selected">Termo Aditivo</option>
        						</select>
        					</div>
                        </div>
                        <?php
                        $sql_modelo = "select
                            				mod.mdoid as codigo,
                            				mod.mdonome as descricao
                        				from par3.modelodocumento mod
                        		     	where mod.mdostatus = 'A' AND mod.mdovisivel <> 'N'
                        					and mod.mdoesfera = '{$arrEntidade['esfera']}'
                        					and mod.mdotipo = '{$mdotipo}'
                        		     		and mod.tpdcod = 21
                                            and mod.mdoex_oficio = 'S'
                            			order by mod.mdonome asc";
                        
                        echo $simec->select('mdoid', 'Modelo de documento', $mdoid, $sql_modelo, array('maxlength' => '255'), array('label-size' => '2', 'input-size' => '9'));
                        echo $simec->textarea('dotjustificativa_de_oficio', 'Justificativa', $dotjustificativa_de_oficio, array('maxlengh' => 4000), ['label-size' => '2', 'input-size' => '9'] );
                        echo $simec->data('dotdata_de_oficio', 'Data', $dotdata_de_oficio, ['autocomplete' => 'off', 'onchange' => 'verificaDataInformada(this.value)'], ['label-size' => '2', 'input-size' => '4']);?>
                        <div class="form-group ">
                            <label for="" class="col-sm-2 col-md-2 col-lg-2 control-label"> </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 " id="div_msg"></div>
                        </div>
                    <td style="text-align: center;" valign="center"><?php
                                echo wf_desenhaBarraNavegacaoBootstrap(
                                        $arrDocumentoTermo['docid'],
                                        array('dotid' => $arrDocumentoTermo['dotid'],
                                            'proid' => $proid),
                                        array(),
                                        'Fluxo',
                                    "trataEnvioWorkflowOficio('".$dotid_filho."', '".$dotid_original."', '".$proid."', '".$inuid."', '".$tipo."')"
                                );
                            ?></td>
                	</tr>
                </table>
                        <div class="form-group" style="text-align: right !important;">
                            <label class="col-sm-2 col-md-2 col-lg-2 control-label  control-label">Iniciativas: </label>
                            <div class="col-sm-10 col-md-10 col-lg-10" style="text-align: center !important;">
                                <?php echo $obDocumento->listarIniciativasProcesso($proid, $arrEntidade['tipo_processo']);?>
                            </div>
                        </div>
                        <div class="form-group " style="text-align: right !important;">
                            <label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">&nbsp; </label>
                            <div class="col-sm-9 col-md-9 col-lg-9" style="text-align: left !important;">
                            	<div id="div_btn_carregar_termo">
                                <input type="button" id="btn-carregar-termo" class="btn btn-primary btn-carregar-termo" onclick="carregarTermoAditivo();" value="Carregar Termo">
                                </div>
                            </div>
                        </div>
                        <div style="clear:both; height: 10px;"></div>
                        <div class="form-group " style="text-align: right !important;">
                            <div class="col-sm-12 col-md-12 col-lg-12" style="text-align: center !important;">
                                <div id="div_conteudo">
                                    <textarea id="mdoconteudo" name="mdoconteudo" rows="30" cols="160" class="minutatinymce"></textarea>
                                </div>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
        <script>
        
        tinyMCE.init({
            // General options
            mode : "textareas",
            theme : "advanced",
            language: "pt",
            editor_selector : "minutatinymce",
            plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

            // Theme options
            theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
            theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
            theme_advanced_buttons3 : "tablecontrols, pagebreak", //",|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
            //theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
            theme_advanced_toolbar_location : "top",
            theme_advanced_toolbar_align : "left",
            theme_advanced_statusbar_location : "",
            theme_advanced_resizing : true,

            // Example content CSS (should be your site CSS)
            //content_css : "css/content.css",

            // Drop lists for link/image/media/template dialogs
            template_external_list_url : "lists/template_list.js",
            external_link_list_url : "lists/link_list.js",
            external_image_list_url : "lists/image_list.js",
            media_external_list_url : "lists/media_list.js",

            // Replace values for the template plugin
            template_replace_values : {
                username : "Some User",
                staffid : "991234"
            }
        });
        
        jQuery(document).ready(function(){
        	jQuery('#mdoconteudo_t').val( jQuery('#div_mdoconteudo').html() );
        	jQuery('#mdoconteudo').val(jQuery('#mdoconteudo_t').val());
            
        	jQuery('#mdoconteudo').val(jQuery('#mdoconteudo_t').val());
        });
        </script>
        <?php
    }
    
    public function verificaRegrasTermos( $dotid, $tipo = '' )
    {        
        $sql = "SELECT count(dotid) FROM par3.documentotermo
                WHERE dotid = {$dotid} AND to_char(dotdatafimvigencia, 'YYYY-MM-DD')::date > to_char(now(), 'YYYY-MM-DD')::date";
        $boVigente = $this->model->pegaUm($sql);
        
        $arIniciativa = array();
        $arPrazo = array();
        
        $arRetorno = array();
        if( $tipo == 'prazo' ){
            $arRetorno = $this->verificaRegrasProrrogacaoPrazo($dotid, $boVigente);
        } elseif( $tipo == 'iniciativa' ){
            $arRetorno = $this->verificaRegrasReprogramacaoIniciativa($dotid, $boVigente);
        } else {
            $arRetorno[] = $this->verificaRegrasReprogramacaoIniciativa($dotid, $boVigente);
            $arRetorno[] = $this->verificaRegrasProrrogacaoPrazo($dotid, $boVigente);
        }
        
        return $arRetorno;        
    }
    
    public function verificaRegrasReprogramacaoIniciativa( $dotid, $boVigente )
    {
        if( (int)$boVigente > (int)0 ){
            return array('reformulacao' => 'iniciativa', 'habilita' => 'S', 'msg' => '');
        } else {
            return array('reformulacao' => 'iniciativa', 'habilita' => 'N', 'msg' => 'Não é possível efetuar a "Reprogramação de Iniciativa" com termo vencido!');
        }
    }
    
    public function verificaRegrasProrrogacaoPrazo( $dotid, $boVigente )
    {
            
        /*$sql = "SELECT (CASE WHEN count(d.docid) > 0 THEN 'S' ELSE 'N' END) FROM workflow.documento d
                	INNER JOIN workflow.historicodocumento hd ON hd.docid = d.docid AND hd.hstid = d.hstid
                WHERE d.tpdid = 352
                	AND hd.aedid IN (5798)";
        $boReprovada = $this->model->pegaUm($sql);
        
        if( $boReprovada == 'S' ){ 
            return array('reformulacao' => 'prazo', 'habilita' => 'N', 'msg' => 'Não é possível efetuar a solicitação de prazo, existe uma solicitação anterior que foi reprovada.');
        } else {*/
            
            $sql = "SELECT dotdatafimvigencia, (dotdatafimvigencia::date - CURRENT_DATE) AS dias, intoid FROM par3.documentotermo WHERE dotid = $dotid;";
            $arTermo = $this->model->pegaLinha($sql);
            
            /*$sql = "SELECT count(cotid) FROM par3.configuracao_termo 
                    WHERE intoid = {$arTermo['intoid']} AND cotstatus = 'A' AND ({$arTermo['dias']} BETWEEN cotdiasfimsolic AND cotdiasinisolic)";*/
            
            $sql = "SELECT cotdiasfimsolic , cotdiasinisolic, to_char(('{$arTermo['dotdatafimvigencia']}'::date - cotdiasfimsolic), 'DD/MM/YYYY') as datafim ,  to_char(('{$arTermo['dotdatafimvigencia']}'::date - cotdiasinisolic), 'DD/MM/YYYY') as datainicio FROM par3.configuracao_termo 
                    WHERE intoid = {$arTermo['intoid']} AND cotstatus = 'A'";
            $arRegra = $this->model->pegaLinha($sql);
            //ver($arRegra, $arTermo['dias']);
            if( !empty($arRegra) ){
                if( $arRegra['cotdiasinisolic'] >= $arTermo['dias'] && $arRegra['cotdiasfimsolic'] <= $arTermo['dias'] ){
                    return array('reformulacao' => 'prazo', 'habilita' => 'S', 'msg' => '');
                } else {
                    return array('reformulacao' => 'prazo', 'habilita' => 'N', 'msg' => 'É possível solicitar prorrogação somente após o dia '.$arRegra['datainicio'].' até o dia '.$arRegra['datafim'].'.');                    
                }
            } else {            
                if( (int)$boVigente > (int)0 ){ #caso o termo esteja vigente, habilita a solicitação
                    return array('reformulacao' => 'prazo', 'habilita' => 'S', 'msg' => '');
                } else {            
                    return array('reformulacao' => 'prazo', 'habilita' => 'N', 'msg' => 'Não é possível efetuar a "Prorrogação de Prazo" com termo vencido!');
                }
            }
        //}
    }
    
    public function verificaResponsabilidadeTermo( $inuid )
    {
        $sql = "SELECT count(ur.rpuid) FROM par3.usuarioresponsabilidade ur
                	INNER JOIN par3.unidade_executora ue ON ue.ungid = ur.ungid AND ue.ungstatus = 'A'
                WHERE ur.rpustatus = 'A' AND ue.inuid = $inuid
                	AND ur.usucpf = '{$_SESSION['usucpf']}'";
        $tem_Resp = $this->model->pegaUm($sql);
        
        $sql = "SELECT muncod, estuf FROM par3.usuarioresponsabilidade ur
                WHERE ur.rpustatus = 'A' AND ur.pflcod in (1436, 1437) --prefeito --secretario estadual
                	AND ur.usucpf = '{$_SESSION['usucpf']}'";
        $arRespo = $this->model->pegaLinha($sql);
        
        if( !empty($arRespo['muncod']) ){
            $e_responsavel = $this->model->pegaUm("SELECT count(inuid) FROM par3.instrumentounidade WHERE muncod = '{$arRespo['muncod']}' AND inustatus = 'A' AND inuid = $inuid");
        } else {
            $e_responsavel = $this->model->pegaUm("SELECT count(inuid) FROM par3.instrumentounidade WHERE estuf = '{$arRespo['estuf']}' AND muncod IS NULL AND inustatus = 'A' AND inuid = $inuid");
        }
        
        if( (int)$tem_Resp > (int)0 || (int)$e_responsavel > (int)0 ){
            return 'S';
        } else {
            return 'N';
        }
    }
}