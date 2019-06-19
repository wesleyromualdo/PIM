<?php
class Par3_Controller_RemanejamentoPagamento extends Controle
{
    protected $model;
    public function __construct()
    {
        $this->model = new Par3_Model_RemanejamentoPagamento();
    }
    
    public function carregarDadosEmpenho($arrPost = array())
    {
        if ($arrPost['tipo'] == 'par') {
            $tabela = 'v_saldo_empenho_composicao_iniciativa';
        }
        else if ($arrPost['tipo'] == 'obra') {
            $tabela = 'v_saldo_empenho_por_obra';
        }
        
        if( $arrPost['numeroprocesso'] ){
            $arrPost['numeroprocesso'] = str_replace(".","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("/","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("-","", $arrPost['numeroprocesso']);
            
            $arrPost['proid'] = $this->model->pegaUm(" SELECT proid FROM par3.processo WHERE pronumeroprocesso = '{$arrPost['numeroprocesso']}'");
        }
        
        $boErro = $arrPost['boErro'];
        
        $funcao = "selecionarEmpenhoPagamento('||ve.empid||');";
        
        $sql = "SELECT distinct
					ve.empid,
                    ve.empid as radio_empid,
				    ve.empenho_original,
				    ve.empfonterecurso,
				    sum(ve.saldo) as vlrempenho,
				    coalesce(sum(ve.vlr_solicitado),0) as vlr_solicitado,
				    coalesce(sum(ve.vlr_efetivado),0) as vlr_efetivado,
				    ve.empsituacao,
				    ve.empprotocolo,
				    formata_cpf_cnpj(ve.entcnpj) as cnpj,
				    ve.entnome
				FROM par3.$tabela ve
				WHERE
					ve.proid = {$arrPost['proid']}
					and ve.saldo > 0
                GROUP BY ve.empid, ve.empenho_original, ve.empfonterecurso, ve.empsituacao, ve.empprotocolo,
				    ve.entcnpj, ve.entnome";
        
        
        $listagemSimec = new Simec_Listagem();
        
        $arrayCabecalho = array('', 'Nº Empenho','Fonte de Recurso', 'Valor Empenhado', 'Pagamento Solicitado', 'Pagamento Efetivado', 'Situação do Empenho', 'Protocolo', 'CNPJ', 'Entidade');
        $esconderColunas = array('empid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['vlrempenho', 'vlr_solicitado', 'vlr_efetivado']);
        $listagemSimec->addCallbackDeCampo(['vlrempenho', 'vlr_solicitado', 'vlr_efetivado'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo('radio_empid','radioEventoEmpenho');
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        
        echo $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function carregarDadosParaPagamento($arrPost = array())
    {
        
        $proid = $arrPost['proid'];
        $empid = $arrPost['empid'];
        
        if ($arrPost['tipo'] == 'par') {
            $cabecalho = array('epcid', 'Iniciativa', 'Valor da Iniciativa', 'Valor Empenhado',  '% Empenhado', 'Valor Pago neste Empenho', '% Pago no Empenho', '% Saldo total deferido');
            $sql = $this->model->carregarDadosParaPagamentoPAR($empid);
        } else {
            $cabecalho = array('eocid', 'Obra', 'Valor da Obra', '% Execução obra', 'Valor Empenhado', '% Empenhado', 'Valor Pago neste Empenho', '% Pago no Empenho', '% Saldo total deferido');
            $sql = $this->model->carregarDadosParaPagamentoOBRA($empid);
        }
        
        $arrDados = $this->model->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        echo getToobarListagem($sql, false);
        $html = '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="0">
				<thead>
					<tr><input type="hidden" name="empid_pag" value="'.$empid.'">';
        foreach ($cabecalho as $v) {
            $html .= '<th valign="middle">'.$v.'</th>';
        }
        $html .= '</tr>
				</thead>
				<tbody>';
        
        $totalIniciativa = 0;
        $totalEmpenhado = 0;
        $totalPagoEmpenho = 0;
        $obPendencia = new Par3_Controller_Pendencia();
        $boPendenciaIniciativa = 'S';
        
        foreach ($arrDados as $v) {
            $totalIniciativa += $v['vlriniciativa'];
            $totalEmpenhado += $v['saldo'];
            $totalPagoEmpenho += $v['vlrpagamento'];
                        
            $v['checkbox'] = false;
            $colunaDesembolso = '<td>' . self::formata_coluna_desembolso($v['saldo_desembolso'], $v) . '</td>';
            
            if ($arrPost['tipo'] == 'obra') {
                $execucaoObra = '<td>'.simec_number_format($v['percent_obra']).'</td>';
                $tdAdicionalObra = '<td></td>';
            }
            
            $html .= '<tr>
						<td>'.$v['codigo'].'</td>
						<td>'.$v['descricaoitem'].'</td>
						<td>'.simec_number_format($v['vlriniciativa']).'</td>
						' . $execucaoObra . '
						<td>'.simec_number_format($v['saldo']).'</td>
						<td>'.number_format_par3($v['percent_empenho']).'%</td>
						<td>'.simec_number_format($v['vlrpagamento']).'</td>
						<td>'.number_format_par3($v['percent_pago']).'%</td>
						' . $colunaDesembolso . '
					</tr>';
        }
        $html .= '</tbody></table>';
        echo $html;
    }
    
    public static function formata_coluna_desembolso($saldo, $campos){
        
        $arrDef = explode(',', $campos['deferimento']);
        $deferimento = '';
        $totalDeferido = 0.00;
        if(count($campos['deferimento'])) {
            foreach ($arrDef as $def) {
                $arrValues = explode('/', $def);
                $id = $arrValues[0];
                $perc_desembolso = trim($arrValues[1]);
                $perc_pag_def = trim($arrValues[2]);
                
                $totalDeferido = bcadd($totalDeferido, $perc_desembolso, 2);
                
                if ($campos['checkbox']) {
                    if($perc_pag_def == '100.00%') {
                        $deferimento .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {$def}";
                    }else{
                        $deferimento .= "<tr>
                                            <td>
                                              <div class='checkbox checkbox-success'>
                                                  <input disabled type='checkbox' name='chkdef[{$id}]' id='chkdef_{$id}'
                                                                  value='{$perc_desembolso}' class='chkdef_{$campos['codigo']}'
                                                                  onchange='alteraValorDesembolso(this,{$campos['codigo']})' >
                                                  <label for='chkdef_{$id}'>&nbsp;</label>
                                                  <input type='hidden' name='pagdef[{$id}]' value='{$perc_pag_def}'>
                                                  
                                              </div>
                                            </td>
                                            <td>{$id}</td>
                                            <td>{$perc_desembolso}%</td>
                                            <td>{$perc_pag_def}%</td>
                                        </tr>";
                    }
                    $cabecalho = '<th style="vertical-align: bottom;">Ação</th>';
                    $tdAdicional = '<td></td>';
                } else {
                    $deferimento .= '<tr><td>' . $id . '</td><td>' . $perc_desembolso . '%</td><td>' . $perc_pag_def . '%</td></tr>';
                }
            }
        }
        
        $saldo = $saldo > 0 ? $saldo : '0.00';
        
        $html = '<style>.popover-header {
                    padding: .5rem .75rem;
                    margin-bottom: 0;
                    font-size: 1rem;
                    color: inherit;
                    background-color: #f7f7f7;
                    border-bottom: 1px solid #ebebeb;
                    border-top-left-radius: calc(.3rem - 1px);
                    border-top-right-radius: calc(.3rem - 1px);
                }</style>';
        
        
        $html .= '<div class="btn-group dropup" >
                  <span type="button" class="btn btn-secondary dropdown-toggle" data-toggle="dropdown" title="Deferimento" aria-haspopup="true" aria-expanded="false">
                    ' . $saldo . '% <i style="color: #64a0e8;" class="fa fa-info-circle"></i>
                  </span>
                  <div class="dropdown-menu" style="padding-bottom: 8px;width: 290px;">
                    <h4 class="popover-header">Deferimento</h4>
                    <div style="padding: 8px 8px 0px 8px;">
                      <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" style="padding: 10px; margin: 0px !important;">
                        <thead><tr>'.$cabecalho.'<th>ID Solicitação</th><th>% Deferido</th><th>% Pago</th></tr></thead>
                   ';
        
        if($deferimento == ''){
            $html .= '
                        <tbody><tr><td colspan="3"><div class="alert alert-info text-center nenhum-registro" id="tb_render">Nenhuma solicitação de desembolso encontrada</div></td></tr></tbody>
                
                
                      ';
        }else{
            $html .= '
                
                            <tbody>' . $deferimento . '</tbody>
                            <tfoot>'.$tdAdicional.'<td><b>Total deferido:</b></td><td>'. $totalDeferido . '%</td><td></td></tfoot>
                        ';
        }
        
        $html .= '</table></div>
                  </div>
                </div>';
        
        return $html;
    }
    
    public function carregarDadosHistoricoPagamento( $arrParr = array() )
    {
        $empid = $arrParr['empid'];
        $proid = $arrParr['proid'];
        $numeroprocesso = $arrParr['numeroprocesso'];
        $tipo = $arrParr['tipo'];
        
        $sql = "SELECT
				    p.pagid,
				    p.pagid as id,
				    e.empnumeroprocesso,
                    e.empid,
				    p.pagparcela,
				    p.pagmes,
				    p.paganoparcela,
				    p.pagnumseqob,
				    p.pagnumeroob,
                    e.empnumero as ne,
				    p.pagvalorparcela,
                    to_char(p.pagdatapagamento,'DD/MM/YYYY HH24:MI:SS'),
				    u.usucpf || ' - ' || u.usunome as usunome,
				    COALESCE(p.pagsituacaopagamento,'-') as situacao,
				    p.paganoexercicio,
                    '{$tipo}' as tipo
				FROM
				    par3.pagamento p
				    INNER JOIN par3.empenho e ON e.empid = p.empid and empstatus = 'A'
				    LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf
				WHERE
				    p.empid in ($empid)
				    and pagstatus='A'
				    and p.pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
				ORDER BY
				    pagparcela";
        
        $listagemSimec = new Simec_Listagem();
        //        $listagemSimec->turnOnPesquisator();
        $arrayCabecalho = array('pagid', 'empid', 'Parcela', 'Mês', 'Ano', 'Seq OB', 'Nº OB', 'NE', 'Valor(R$)', 'Data', 'Situação pagamento');
        $esconderColunas = array('empnumeroprocesso', 'paganoexercicio', 'usunome', 'tipo');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        //$listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('plus', array('func' => 'detalharHistoricoPagamento', 'extra-params' => array('tipo', 'empid') ));
        $listagemSimec->addAcao('new', array('func' => 'abrirModalPagamento', 'extra-params' => array('tipo', 'empid'), 'cor' => 'success'));
        $listagemSimec->addCallbackDeCampo('pagvalorparcela', 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, 'pagvalorparcela');
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    function carregarComposicaoPagamento( $pagid, $tipo_processo, $empid )
    {
        if( $tipo_processo == 'obra' ){
            $sql = "SELECT pmcid as ppcid, pagid, eocid as epcid, pmcpercentualpag as ppcpercentualpag, pmcvalorpagamento as ppcvalorpagamento, pmcstatus as ppcstatus
                    FROM par3.pagamentoobracomposicao WHERE pagid = $pagid";
            $htmlComposicao = '<th class=""></th>
        			<th class="">pmcid</th>
        			<th class="">pagid</th>
        			<th class="">eocid</th>
        			<th class="">pmcpercentualpag</th>
        			<th class="">pmcvalorpagamento</th>
        			<th class="">pmcstatus</th>';
        } else {
            $sql = "SELECT ppcid, pagid, epcid, ppcpercentualpag, ppcvalorpagamento, ppcstatus
                    FROM par3.pagamentoparcomposicao WHERE pagid = $pagid";
            
            $htmlComposicao = '<th class=""></th>
        			<th class="">ppcid</th>
        			<th class="">pagid</th>
        			<th class="">epcid</th>
        			<th class="">ppcpercentualpag</th>
        			<th class="">ppcvalorpagamento</th>
        			<th class="">ppcstatus</th>';
        }        
        $arrEmpenho = $this->model->carregar($sql);
        $arrEmpenho = $arrEmpenho ? $arrEmpenho : array();
        
        /*$proid = $this->model->pegaUm("SELECT p.proid FROM par3.processo p
                                        	INNER JOIN par3.empenho e ON e.empnumeroprocesso = p.pronumeroprocesso
                                        WHERE e.empid = {$empid}");*/
        
        $html = '
        <form name="form-pagamento-'.$pagid.'" id="form-pagamento-'.$pagid.'" class="form-horizontal" method="post">
		<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="1">
        	<thead>
        		<tr>
        			'.$htmlComposicao.'
        		</tr>
        	</thead>
        	<tbody>';
        $html1 = '';
        $totalEmpenho = 0;
        foreach ($arrEmpenho as $v) {
            $totalEmpenho = ($totalEmpenho + $v['ppcvalorpagamento']);
            $html1 .= '
        		<tr>
                    <td class="text-center" style="width:33px"><a href="javascript:excluirComposicao('.$v['ppcid'].', '.$empid.');" title="Excluir Composição"><span class="btn btn-danger btn-sm glyphicon glyphicon-remove"></span></a></td>
        			<td><input name="ppcid['.$v['ppcid'].']" id="ppcid" type="text" value="'.$v['ppcid'].'" class="form-control" maxlength="255" style="width: 100px;" readonly="readonly"
        				onkeyup="this.value=mascaraglobal(\'[#]\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);"></td>
        			<td><input name="pagid['.$v['ppcid'].']" id="pagid" type="text" value="'.$v['pagid'].'" class="form-control" maxlength="255" style="width: 100px;"
        				onkeyup="this.value=mascaraglobal(\'[#]\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);"></td>
        			<td><input name="epcid['.$v['ppcid'].']" id="epcid" type="text" value="'.$v['epcid'].'" class="form-control" maxlength="255" style="width: 100px;"
        				onkeyup="this.value=mascaraglobal(\'[#]\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);"></td>
        			<td><input name="ppcpercentualpag['.$v['ppcid'].']" id="ppcpercentualpag" type="text" value="'.$v['ppcpercentualpag'].'" class="form-control" maxlength="255" style="width: 100px;"
        				onkeyup="this.value=mascaraglobal(\'[#]\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);"></td>
        			<td><input name="ppcvalorpagamento['.$v['ppcid'].']" id="ppcvalorpagamento" type="text" value="'.simec_number_format($v['ppcvalorpagamento'], 2, ",", ".").'" class="form-control" maxlength="255" style="width: 200px;"
        				onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[.###],##\',this.value);"></td>
        			<td><input name="ppcstatus['.$v['ppcid'].']" id="ppcstatus" type="text" value="'.$v['ppcstatus'].'" class="form-control" maxlength="255" style="width: 100px;"></td>
        		</tr>';
        }
        $html .= $html1.'
                <tr>
                    <td colspan=5 style="text-align: right;"><b>Total:</b></td>
                    <td style="text-align: left;">'.simec_number_format($totalEmpenho, 2, ",", ".").'</td>
                    <td></td>
                </tr>
                <tr>
                    <td colspan="10"><button type="button" id="salvar_composicao" class="btn btn-success" onclick="salvarcomposicaoPar('.$pagid.');"><i class="fa fa-plus-square-o"></i> Salvar Composição</button></td>
                </tr>
        	</tbody>
        </table></form>';
        echo $html;
    }
}











