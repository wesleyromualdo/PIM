<?php
class Par3_Controller_RemanejamentoEmpenho extends Controle
{
    protected $model;
    public function __construct()
    {
        $this->model = new Par3_Model_RemanejamentoEmpenho();
    }
    
    public function listaComposicaoProcesso( $arrPost = array() )
    {        
        
        $sql = $this->model->sqlListaComposicaoProcesso($arrPost);
        
        $iniciativas = $this->model->carregar($sql);
        echo getToobarListagem($sql, false);
        $html = '
        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">
        <thead>
        <tr>
            <th>ppcid</th>
            <th>inpid</th>
            <th>Ano</th>
            <th>Descrição</th>
            <th>Valor Total</th>
            <th>Valor de Contrapartida</th>
            <th>Valor FNDE</th>
            <th>% Empenhado</th>
            <th>Valor Empenhado</th>
            <th>Valor Disponível</th>
            <th>Valor Pago</th>
            <th>Situação</th>';        
        if( $arrPost['tipo_processo'] == 'P' ){
            $html .= '<th>anaid processo</th>
                     <th>anaid analise</th>';
        }
        $html .= '</tr>
            </thead>
            <tbody>';
        
        if ($iniciativas[0]) {
            foreach ($iniciativas as $ini) {
                $cor = '';
                if( $ini['saldo_disponivel'] != 0 ){
                    $cor = ' style="color: blue;" ';
                }
                
                $html .= '<tr '.$cor.'>';
                $html .= '<td>'.$ini['id'].'</td>';
                $html .= '<td>'.$ini['codigo'].'</td>';
                $html .= '<td>'.$ini['anoitem'].'</td>';
                $html .= '<td>'.$ini['descricaoitem'].'</td>';
                $html .= '<td>'.simec_number_format($ini['totalitem'], 2, ",", ".").'<input type="hidden" id="totalitem_'.$ini['id'].'" value="'.($ini['totalitem'] - $ini['contrapartida']).'"></td>';
                $html .= '<td>'.simec_number_format($ini['contrapartida'], 2, ",", ".").'<input type="hidden" id="contrapartida_'.$ini['id'].'" value="'.$ini['contrapartida'].'"></td>';
                $html .= '<td>'.simec_number_format($ini['valorfnde'], 2, ",", ".").'</td>';
                $html .= '<td>'.number_format_par3($ini['percentual']).'%<input type="hidden" id="porcempenhado_'.$ini['id'].'" value="'.$ini['percentual'].'"></td>';
                $html .= '<td>'.simec_number_format($ini['valorempenhado'], 2, ",", ".").'<input type="hidden" id="valorempenhado_'.$ini['id'].'" value="'.$ini['valorempenhado'].'"></td>';
                $html .= '<td>'.simec_number_format($ini['saldo_disponivel'], 2, ",", ".").'<input type="hidden" id="saldo_disponivel_'.$ini['id'].'" value="'.$ini['saldo_disponivel'].'"></td>';
                $html .= '<td>'.simec_number_format($ini['vlrpago'], 2, ",", ".").'<input type="hidden" id="vlrpago_'.$ini['id'].'" value="'.$ini['vlrpago'].'"></td>';
                $html .= '<td>'.$ini['esddsc'].'</td>';
                if( $arrPost['tipo_processo'] == 'P' ){
                    $html .= '<td>'.$ini['anaid_processo'].'</td>';
                    $html .= '<td>'.$ini['anaid_analise'].'</td>';
                }
                $html .= '</tr>';
            }
        }
        $html .= '</tbody>
                  </table>';
        
        echo $html;
    }
    public function listaEmpenhoProcesso( $arrPost = array() )
    {
        $where = '';
        if( $arrPost['proid'] ) $where .= " and v.proid = {$arrPost['proid']} ";
        if( $arrPost['numeroprocesso'] ){
            $arrPost['numeroprocesso'] = str_replace(".","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("/","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("-","", $arrPost['numeroprocesso']);
            
            $where .= " and v.proid = (SELECT proid FROM par3.processo WHERE pronumeroprocesso = '{$arrPost['numeroprocesso']}') ";
        }
        
        if( $arrPost['tipo_processo'] == 'P' ){
            $inner = "INNER JOIN par3.processoparcomposicao pp ON pp.proid = v.proid
                      INNER JOIN(                	
                		SELECT empid, sum(epcvalorempenho) AS vlr_composicao FROM par3.empenhoparcomposicao WHERE epcstatus = 'A' GROUP BY empid                	
                	) ep ON ep.empid = e.empid ";
        } else {
            $inner = "INNER JOIN par3.processoobracomposicao pp ON pp.proid = v.proid
                      INNER JOIN(                	
                    		SELECT empid, sum(o.eocvalorempenho) AS vlr_composicao FROM par3.empenhoobracomposicao o WHERE o.eocstatus = 'A' GROUP BY empid                	
                    	) ep ON ep.empid = e.empid";
        }
        $sql = "SELECT DISTINCT e.empid, COALESCE(e.empnumero,'-') as numeroempenho,
                    (SELECT empnumero FROM par3.empenho WHERE empid = e.empidpai) as empenho_cancelado,
                    e.empsituacao AS situacaoempenho, ''||u.usunome||' - '||to_char(e.empdata,'dd/mm/YYYY HH24:MI')||'' as usuario,
                	formata_cpf_cnpj(e.empcnpj) as cnpj, COALESCE(e.empprotocolo,'-') as numeroprotocolo,
                	e.empvalorempenho AS vlr_empenho,
                    ep.vlr_composicao,
                    (e.empvalorempenho - ep.vlr_composicao) as divergencia,
                	coalesce(ec.vlr_cancelado,0) AS vlr_cancelado,
                	(ep.vlr_composicao - coalesce(ec.vlr_cancelado,0))::numeric(20,2) as saldo,
                	coalesce(pg.vlr_pagamento,0) AS vlr_pagamento, '".$arrPost['tipo_processo']."' as tipo_processo
                FROM par3.empenho e
                	INNER JOIN par3.processo v ON v.pronumeroprocesso = e.empnumeroprocesso
                    $inner
                	LEFT JOIN seguranca.usuario u ON u.usucpf = e.usucpf
                	LEFT JOIN(
                		SELECT empidpai, sum(empvalorempenho) AS vlr_cancelado
                		FROM par3.empenho e
                		WHERE e.empcodigoespecie in ('03', '13', '04') AND e.empstatus = 'A'
                		GROUP BY empidpai
                	) ec ON ec.empidpai = e.empid
                	LEFT JOIN (
                		SELECT empid, sum(pagvalorparcela) AS vlr_pagamento FROM par3.pagamento
                		WHERE pagstatus = 'A'
                			and pagsituacaopagamento not ilike '%CANCELADO%'
                			and pagsituacaopagamento not ilike '%vala%'
                			and pagsituacaopagamento not ilike '%devolvido%'
                            and pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
                		GROUP BY empid
                	) pg ON pg.empid = e.empid
                WHERE e.empstatus = 'A' $where
                ORDER BY 1,2";
                    
        $arrayCabecalho = array('Nota do Empenho', 'Empenho Pai', 'Situação do Empenho', 'Número do Protocolo', 'Vlr Empenhado', 'Vlr Composição', 'Divergência', 'Vlr Cancelado', 'Saldo', 'Vlr Pago');
        $esconderColunas = array('tipo_processo', 'usuario', 'cnpj');
        
        $listagemSimec = new Simec_Listagem();
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('plus', array('func' => 'detalharEmpenho', 'extra-params' => array('tipo_processo') ));
        $listagemSimec->addAcao('new', array('func' => 'abrirModalEmpenho', 'extra-params' => array('tipo_processo'), 'cor' => 'success'));
        $listagemSimec->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
        $listagemSimec->addCallbackDeCampo(['vlr_empenho', 'valorsaldo', 'vlr_pagamento', 'vlr_cancelado', 'divergencia', 'vlr_composicao', 'saldo'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo(['divergencia'], 'formataValorDivergenteEmpenho');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    function detalharEmpenhoComposicao( $empid, $tipo_processo )
    {
        if( $tipo_processo == 'P' ){
            $sql = "SELECT epcid as id, epcid, empid, ppcid, epcpercentualemp, epcvalorempenho, epcstatus
                    FROM par3.empenhoparcomposicao WHERE empid = {$empid}";
        } else {
            $sql = "SELECT eocid as id, eocid as epcid, empid, pocid as ppcid, eocpercentualemp as epcpercentualemp, eocvalorempenho as epcvalorempenho, eocstatus as epcstatus
                    FROM par3.empenhoobracomposicao WHERE empid = {$empid}";
        }
        
        $arrEmpenho = $this->model->carregar($sql);
        $arrEmpenho = $arrEmpenho ? $arrEmpenho : array();
        
        $proid = $this->model->pegaUm("SELECT p.proid FROM par3.processo p
                                        	INNER JOIN par3.empenho e ON e.empnumeroprocesso = p.pronumeroprocesso
                                        WHERE e.empid = {$empid}");
        
        $html = '
        <form name="form-empenho-'.$empid.'" id="form-empenho-'.$empid.'" class="form-horizontal" method="post">
            	<input type="hidden" name="proid" id="proid" value="'.$proid.'"/>
            	<input type="hidden" name="tipo_processo" id="tipo_processo" value="'.$tipo_processo.'"/>
		<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="1">
        	<thead>
        		<tr>
        			<th class=""></th>
        			<th class="">epcid</th>
        			<th class="">empid</th>
        			<th class="">ppcid</th>
        			<th class="">epcpercentualemp</th>
        			<th class="">epcvalorempenho</th>
        			<th class="">epcstatus</th>
        		</tr>
        	</thead>
        	<tbody>';
        $html1 = '';
        $totalEmpenho = 0;
        foreach ($arrEmpenho as $v) {
            $totalEmpenho = ($totalEmpenho + $v['epcvalorempenho']);
            $html1 .= '
        		<tr>
                    <td class="text-center" style="width:33px"><a href="javascript:excluirComposicao('.$v['epcid'].', '.$v['empid'].');" title="Excluir Composição"><span class="btn btn-danger btn-sm glyphicon glyphicon-remove"></span></a></td>
        			<td><input name="epcid['.$v['epcid'].']" id="epcid" type="text" value="'.$v['epcid'].'" class="form-control class_epcid" maxlength="255" style="width: 100px;" readonly="readonly"
        				onkeyup="this.value=mascaraglobal(\'[#]\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);"></td>
        			<td><input name="empid['.$v['epcid'].']" id="empid" type="text" value="'.$v['empid'].'" class="form-control" maxlength="255" style="width: 100px;"
        				onkeyup="this.value=mascaraglobal(\'[#]\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);"></td>
        			<td><input name="ppcid['.$v['epcid'].']" id="ppcid" type="text" value="'.$v['ppcid'].'" class="form-control" maxlength="255" style="width: 100px;"
        				onkeyup="this.value=mascaraglobal(\'[#]\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);"></td>
        			<td><input name="epcpercentualemp['.$v['epcid'].']" id="epcpercentualemp" type="text" value="'.$v['epcpercentualemp'].'" class="form-control" maxlength="255" style="width: 100px;"
        				onkeyup="this.value=mascaraglobal(\'[#]\',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);"></td>
        			<td><input name="epcvalorempenho['.$v['epcid'].']" id="epcvalorempenho" type="text" value="'.simec_number_format($v['epcvalorempenho'], 2, ",", ".").'" class="form-control" maxlength="255" style="width: 200px;"
        				onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value); calculaTotalEmpenho('.$v['epcid'].', '.$v['empid'].');" onblur="MouseBlur(this);this.value=mascaraglobal(\'[.###],##\',this.value); calculaTotalEmpenho('.$v['epcid'].', '.$v['empid'].');"></td>
        			<td><input name="epcstatus['.$v['epcid'].']" id="epcstatus" type="text" value="'.$v['epcstatus'].'" class="form-control" maxlength="255" style="width: 100px;"></td>
        		</tr>';
        }
        $html .= $html1.'
                <tr>
                    <td colspan=5 style="text-align: right;"><b>Total:</b></td>
                    <td style="text-align: left;" id="td_total_empenho">'.simec_number_format($totalEmpenho, 2, ",", ".").'</td>
                    <td></td>
                </tr>
                <tr>
                    <td colspan="10"><button type="button" id="salvar_composicao" class="btn btn-success" onclick="salvarcomposicaoPar('.$empid.');"><i class="fa fa-plus-square-o"></i> Salvar Composição</button></td>
                </tr>
        	</tbody>
        </table></form>';
        echo $html;
    }
    
    public function listaComposicaoItemDisponivel( $arrPost )
    {
        $sql = $this->model->sqlListaComposicaoProcesso($arrPost);
        
        $iniciativas = $this->model->carregar($sql);
        echo getToobarListagem($sql, false);
        $html = '
        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">
        <thead>
        <tr>
            <th>ppcid</th>
            <th>inpid</th>
            <th>Ano</th>
            <th>Valor Total</th>
            <th>Valor de Contrapartida</th>
            <th>Valor FNDE</th>
            <th>% Empenhado</th>
            <th>Valor Empenhado</th>
            <th>Valor Disponível</th>
            <th>Valor Pago</th>
        </tr>
        </thead>
        <tbody>';
        
        if ($iniciativas[0]) {
            foreach ($iniciativas as $ini) {
                
                if( $ini['saldo_disponivel'] > 0 ){                
                    $html .= '<tr '.$cor.'>';
                    $html .= '<td>'.$ini['id'].'</td>';
                    $html .= '<td>'.$ini['codigo'].'</td>';
                    $html .= '<td>'.$ini['anoitem'].'</td>';
                    $html .= '<td>'.simec_number_format($ini['totalitem'], 2, ",", ".").'<input type="hidden" id="totalitem_'.$ini['id'].'" value="'.($ini['totalitem'] - $ini['contrapartida']).'"></td>';
                    $html .= '<td>'.simec_number_format($ini['contrapartida'], 2, ",", ".").'<input type="hidden" id="contrapartida_'.$ini['id'].'" value="'.$ini['contrapartida'].'"></td>';
                    $html .= '<td>'.simec_number_format($ini['valorfnde'], 2, ",", ".").'</td>';
                    $html .= '<td>'.number_format_par3($ini['percentual']).'%<input type="hidden" id="porcempenhado_'.$ini['id'].'" value="'.$ini['percentual'].'"></td>';
                    $html .= '<td>'.simec_number_format($ini['valorempenhado'], 2, ",", ".").'<input type="hidden" id="valorempenhado_'.$ini['id'].'" value="'.$ini['valorempenhado'].'"></td>';
                    $html .= '<td style="color: blue;">'.simec_number_format($ini['saldo_disponivel'], 2, ",", ".").'<input type="hidden" id="saldo_disponivel_'.$ini['id'].'" value="'.$ini['saldo_disponivel'].'"></td>';
                    $html .= '<td>'.simec_number_format($ini['vlrpago'], 2, ",", ".").'<input type="hidden" id="vlrpago_'.$ini['id'].'" value="'.$ini['vlrpago'].'"></td>';
                    $html .= '</tr>';
                }
            }
        }
        $html .= '</tbody>
                  </table>';
        
        echo $html;
    }
}











