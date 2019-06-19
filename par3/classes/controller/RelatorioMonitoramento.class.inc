<?php
class Par3_Controller_RelatorioMonitoramento extends Controle
{
	public function listaMonitoramento($arrPost)
    { 

        $sql = $this->montaSQLMONITORAMENTO($arrPost);

        //Cabeçalho: código,descrição,situção
       $cabecalho = array('UF', 'Municipio', 'Situação', 'Pendências');

        $esconderColunas = array('lista');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('lista');
           // array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_CORRIDO;
        
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('areas');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        if (!$arrPost['req']) {
            $listagem->addCallbackDeCampo('lista','sanitizar_string_pesquisa');
        }
        $listagem->addAcao('plus', array('func' => 'detalhesPendenciasMonitoramento'));
        $listagem->turnOnOrdenacao('municipio');
        $listagem->render($tratamentoListaVazia);
    }

    public function listaMonitoramentoDetalhado($arrPost)
    { 

        $sql = $arrPost['sql'];

        //Cabeçalho: código,descrição,situção
       $cabecalho = $arrPost['cabecalho'];

        $esconderColunas = $arrPost['esconderColunas'];

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('areas');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('lista','sanitizar_string_pesquisa');
        $listagem->setTamanhoPagina(50);
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }


    public function montaSQLMONITORAMENTO($arrPost)
    {
      set_time_limit(300);
        $where = array('1=1');
        if ($arrPost['esfera'] == 'E')      $where[] = "inu.itrid IN (1,3)";
        if ($arrPost['esfera'] == 'M')      $where[] = "inu.itrid IN (2)";
        if ($arrPost['pendencia'] == 'P')   $where[] = "vm.monitoramento_par IS TRUE";
        if ($arrPost['pendencia'] == 'N')   $where[] = "vm.monitoramento_par IS NOT TRUE";
        if ($arrPost['estuf'])              $where[] = "inu.estuf = '{$arrPost['estuf']}'";
        if ($arrPost['muncod'])             $where[] = "inu.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $where[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $where[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }

        /** TRATAMENTO PARA AS INFORMAÇÕES DA QUERY recuperaProcessosRegraMonitoramento2010 */
        if($arrPost['esfera'] == 'M'){
            $sqlConsultaInuid = "
                SELECT inu.inuid, inu.muncod, par.inuid as inuidpar 
                FROM par3.vm_relatorio_quantitativo_pendencias vm
                INNER JOIN par3.instrumentounidade  inu ON inu.inuid = vm.inuid
                INNER JOIN par.instrumentounidade   par ON par.muncod = inu.muncod 
                $innerDoc
                WHERE
                     " . implode(' AND ', $where) . "";
         }else{
             $sqlConsultaInuid = "
                SELECT inu.inuid, inu.muncod, par.inuid as inuidpar 
                FROM par3.vm_relatorio_quantitativo_pendencias vm
                INNER JOIN par3.instrumentounidade  inu ON inu.inuid = vm.inuid
                INNER JOIN par.instrumentounidade   par ON par.estuf = inu.estuf 
                $innerDoc
                WHERE
                     " . implode(' AND ', $where) . ""; 
         }
       
        $modal = new Modelo();
        $dados = $modal->carregar($sqlConsultaInuid);

        // se o busca for dos que não tenha pendência, retorne a query abaixo
        if ($arrPost['esfera'] == 'E')  $wherePendenciaN[] = "inu.itrid IN (1,3)";
        if ($arrPost['esfera'] == 'M')  $wherePendenciaN[] = "inu.itrid IN (2)";
        if ($arrPost['estuf'])          $wherePendenciaN[] = "inu.estuf = '{$arrPost['estuf']}'";
        if ($arrPost['muncod'])         $wherePendenciaN[] = "inu.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $wherePendenciaN[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $wherePendenciaN[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }
        
        $sqlPendenciaN =  "
            SELECT DISTINCT
                inu.inuid || '-' || 'monitoramento' || '-' || 'PendenciaMonitoramento' as pendencia, 
                inu.estuf,
                CASE WHEN inu.itrid = 2
                    THEN inu.inudescricao
                    ELSE ' - '
                END as municipio,
                $colunaSituacao
                ' - ' as lista_1
            FROM par3.instrumentounidade inu
            LEFT JOIN par3.vm_relatorio_quantitativo_pendencias vm ON inu.inuid = vm.inuid
            $innerDoc
            LEFT JOIN par.instrumentounidade par ON (par.muncod = inu.muncod AND par.itrid = 2) OR (par.estuf = inu.estuf AND par.muncod IS NULL AND par.itrid <> 2) 
            WHERE (vm.monitoramento_par IS NOT TRUE OR vm.monitoramento_par IS NULL) AND " . implode(' AND ', $wherePendenciaN) . " ";
        
        if ($arrPost['pendencia'] == 'N'){
            return $sqlPendenciaN;
        }

        //CASO NÃO VENHA DADOS, POPULE OS ARRAY COM 0.
        if(!$dados){ 
            $arrCompletoMuncod[] = 0;
            $arrInuid[] = 0;
            $inuidVerificados[] = 0;
        }else{
               $arrInuid = array();
               $cont=0;
               foreach($dados as $key=>$dado){
                    if(count($dado['muncod']) != NULL) {
                        $arrCompletoMuncod[] = $dado['muncod'];
                    }
                    $arrInuid[$cont] = $dado['inuidpar']; 
                    $inuidVerificados[$cont] = $dado['inuid'];
                    $cont++;
                }
        }

        

        if($arrPost['esfera'] == 'M'){
            $whereRegraSaldoConta = "SELECT estuf FROM par.instrumentounidade WHERE itrid = inu.itrid AND muncod = inu.muncod";
            $whereInternaRegraMonitoramento2010 = "mun_estuf = '".$arrPost['estuf']."'";
            $whereExternaRegraMonitoramento2010 = "  interno1.muncod = inu.muncod";
            $whereProcessosRegraPagamento = "  AND iu.muncod = inu.muncod";
        }else{
            $whereRegraSaldoConta = "SELECT estuf FROM par.instrumentounidade WHERE itrid = inu.itrid AND estuf = inu.estuf";
            $whereInternaRegraMonitoramento2010 = "estuf = '".$arrPost['estuf']."'";
            $whereExternaRegraMonitoramento2010 = " interno1.estuf = inu.estuf";
            $whereProcessosRegraPagamento = "";
        }

        if ($arrPost['esfera'] == 'M'){
             $whereMuncod = " AND iu.muncod = (SELECT muncod FROM par.instrumentounidade WHERE itrid = inu.itrid AND muncod = inu.muncod)";
        } else{
            $whereMuncod = '';
        }

        $inuidCertos = implode(' , ', $inuidVerificados);
        
        /** FIM TRATAMENTO recuperaProcessosRegraMonitoramento2010*/
        
        $sql = "/** recuperaProcessosRegraPagamento */
                SELECT DISTINCT
                	inu.pendencia,
                	inu.estuf,
                	inu.municipio,
                	$colunaSituacao
                	inu.lista_1
                FROM par3.vm_relatorio_qualitativo_nivel_1 inu
                $innerDoc
                WHERE
                     ".implode(' AND ', $where);
            
         if ($arrPost['pendencia'] == 'T'){
            return "SELECT DISTINCT * FROM ($sql UNION ALL $sqlPendenciaN) as foo";
         }else{
            return $sql." ORDER BY estuf, municipio";
         }
    }

}