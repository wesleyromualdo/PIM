<?php
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
/**
 * Classe de mapeamento da entidade sase.agendacomissaocoordestado.
 *
 * @version $Id$
 * @since 2016.09.01
 */

/**
 * Sase_Model_Agendacomissaocoordestado: sem descricao
 *
 * @package Sase\Model
 * @uses Simec\Db\Modelo
 * @author Marilúcia Cardozo De Queiroz <marilucia.queiroz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Sase_Model_Agendacomissaocoordestado($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Sase_Model_Agendacomissaocoordestado($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property \Datetime(Y-m-d H:i:s) $acedataalteracao
 * @property \Datetime(Y-m-d H:i:s) $acedatainclusao
 * @property string $usucpf
 * @property string $acestatus  - default: 'A'::bpchar
 * @property string $aceobservacao
 * @property \Datetime(Y-m-d H:i:s) $aceprazo
 * @property string $aceresponsavel
 * @property string $aceacao
 * @property string $estuf
 * @property int $eacid
 * @property int $aceid  - default: nextval('sase.agendacomissaocoordestado_aceid_seq'::regclass)
 */
class Agendacomissaocoordestado extends Modelo
{
    /**
     * @var string Nome da tabe$filename = "test.csv";
     * $csvArray = ImportCSV2Array($filename)
     *
     * foreach ($csvArray as $row)
     * {
     * echo $row['column1'];
     * echo $row['column2'];
     * echo $row['column3'];
     * }la mapeada.
     */
    protected $stNomeTabela = 'sase.agendacomissaocoordestado';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aceid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'eacid' => array('tabela' => 'sase.etapaagendacomcoord', 'pk' => 'eacid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'acedataalteracao' => null,
        'acedatainclusao' => null,
        'usucpf' => null,
        'acestatus' => null,
        'aceobservacao' => null,
        'aceprazo' => null,
        'aceresponsavel' => null,
        'aceacao' => null,
        'estuf' => null,
        'eacid' => null,
        'aceid' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'acedataalteracao' => array('allowEmpty' => true),
            'acedatainclusao' => array('allowEmpty' => true),
            'usucpf' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
            'acestatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'aceobservacao' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1000))),
            'aceprazo' => array('allowEmpty' => true),
            'aceresponsavel' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 300))),
            'aceacao' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1000))),
            'estuf' => array(new Zend_Validate_StringLength(array('max' => 2))),
            'eacid' => array('allowEmpty' => true, 'Digits'),
            'aceid' => array('Digits'),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /**
     * Método de lista os cadastros inseridos na agenda estado.
     * Este método tem as seguintes finalidade: Listar registros
     * @return array
     */
    public function listagemEstadoAgenda($dados)
    {
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

        //filtros
        if ($dados['estuf'] != '') {
            $and .= " AND te.estuf = '{$dados['estuf']}'";
        }
        if ($dados['preenchido'] == 'S') {
            $and .= " AND acedatainclusao IS NOT NULL
                      AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado acc where eacid  = 1 and acestatus = 'A' AND acc.estuf = te.estuf AND ( aceacao != '' AND aceresponsavel != '' AND aceprazo IS NOT NULL) ) )
                      --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 1 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                      --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 2 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                      --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 3 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                      --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 4 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                    ";
        } elseif ($dados['preenchido'] == 'N') {
            $and .= " AND acedatainclusao IS NULL ";
        }


        if (in_array(PFLCOD_SASE_TECNICO, $perfis)) {
            $or = " INNER JOIN territorios.municipio tm ON tm.estuf = ur.estuf OR tm.muncod = ur.muncod";//existem perfis que estão vinculado somente ao município
            $and .= " AND ur.rpustatus = 'A' AND ur.usucpf = '{$_SESSION['usucpf']}'  {$_and}";
        }

        if (in_array(PFLCOD_SASE_EXECUTIVO, $perfis) || in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)) {
            //carrega os municípios atribuídos ao usuário técnico logado.
            $and .= " AND ur.rpustatus = 'A' AND ur.usucpf = '{$_SESSION['usucpf']}'  {$_and}";
        }

        $SQL = "
           SELECT  DISTINCT te.estuf AS acao,
                    te.estuf||' - '||estdescricao as estado,
                    CASE WHEN acedatainclusao IS NOT NULL
                            AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado acc where eacid  = 1 and acestatus = 'A' AND acc.estuf = te.estuf AND ( aceacao != '' AND aceresponsavel != '' AND aceprazo IS NOT NULL) ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 1 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 2 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 3 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 4 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                        THEN 'S'
                        ELSE 'N'
                    END AS preenchido
	    FROM
		sase.usuarioresponsabilidade ur
	     {$or}
            LEFT JOIN	
		territorios.estado te ON te.estuf = ur.estuf
	    LEFT JOIN 
		sase.agendacomissaocoordestado  acce ON acce.estuf = ur.estuf
            WHERE
                1 = 1 
            {$and} 
            ORDER BY 2, 1
        ";
//ver($SQL,d);
        $cabecalho = array('Estado', 'Todas as Etapas Preenchidas');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($SQL);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('preenchido', 'pegaImagem');
        $listagem->addAcao('edit', 'editar_agenda');

        $listagem->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    }

    public function getDadosTrabalhosAgendados($estuf, $etapa)
    {
        $sql = "
                    SELECT  aceid,
                            eacid,
                            trim(aceacao) AS aceacao,
                            trim(aceresponsavel) AS aceresponsavel,
                            to_char(aceprazo, 'DD/MM/YYYY') AS aceprazo,
                            trim(aceobservacao) AS aceobservacao,
                             to_char(aceconcluida, 'DD/MM/YYYY') AS aceconcluida,                             
                            CAST(aceconcluida AS DATE) + INTERVAL '30 DAYS' AS datalimit,
                            to_char(acedatainclusao, 'YYYY') AS acedatainclusao,
                             acedatainclusao as datadesc
                    FROM sase.agendacomissaocoordestado
                    WHERE acestatus = 'A' AND estuf = '{$estuf}' AND eacid = {$etapa}
                    ORDER BY datadesc DESC  
                ";

        return $this->carregar($sql);
    }

    //carrega dados por etapa
    public function ListaTrabalhosAgendados($estuf, $etapa)
    {
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);
        if (!empty($estuf)) {
            $res = $this->getDadosTrabalhosAgendados($estuf, $etapa);
        }
        $ano_passado =  (date("Y")-1);
        $contN = 0;
        if (is_array($res)) {
            $contN = count($res);
        }

        $html_bt_plus = <<<HTML
                <tr id="lineExcluir_{$etapa}_{i}"><td style="vertical-align: middle;"><center><button title="excluir" class="btn btn-danger" type="button" frm="{$etapa}_{i}" onclick="excluirLinha(this)"> x </button><center><input type="hidden" name="reg[]" id="reg{i}" value="{i}"/><input type="hidden" name="aceid[]" id="aceid{i}" value="0"/> </td><td><textarea class="form-control frm-obrigatorio-{$etapa}" id="aceacao{i}" maxlength="1000" name="aceacao[]" rows="2"></textarea></td><td><textarea class="form-control frm-obrigatorio-{$etapa}" id="aceresponsavel{i}" maxlength="300" name="aceresponsavel[]" rows="2"></textarea></td><td><center><input id="aceprazo_{$etapa}_{i}" name="aceprazo_{$etapa}[]" type="text" class="campoData form-control frm-obrigatorio-{$etapa}" value="" placeholder=""></center></td><td><textarea class="form-control" id="aceobservacao{i}" maxlength="1000" name="aceobservacao[]" rows="2"></textarea></td><td><center><input id="aceconcluida_{$etapa}_{i}" name="aceconcluida_{$etapa}[]" type="text" disabled class="campoData form-control " value="" placeholder=""></center></td><td><input type="hidden" name="atualiza[]" id="atualiza{i}" value="N"/></td></tr>
HTML;
        $html = "
                <script>
                    $(document).ready(function(){
                        jQuery(\".campoData\").mask(\"99/99/9999\");
                        jQuery(\".campoData\").datepicker();
                    });

                    function adicionaAtividade_{$etapa}(){
                        c = jQuery('#formulario_'+{$etapa}).find(\"#contN\").val(); //jQuery(\"#contN\").val();
                        console.log(c);
                        t = '{$html_bt_plus}';
                        c++;
                        t = t.replace(new RegExp(\"{i}\", \"g\"), c);
                        console.log(t);
                        jQuery('#formulario_'+{$etapa}).find(\"#camposN\").append(t); //jQuery(\"#camposN\").append(t);
                        jQuery('#formulario_'+{$etapa}).find(\"#contN\").val(c); //jQuery(\"#contN\").val(c);
                        jQuery('#formulario_'+{$etapa}).find(\".campoData\").mask(\"99/99/9999\"); //jQuery(\".campoData\").mask(\"99/99/9999\");
                        jQuery('#formulario_'+{$etapa}).find(\".campoData\").datepicker(); //jQuery(\".campoData\").datepicker();
                    }
                    
                </script>
                <style>
                    .campoData{
                        width: 100px !important;
                    }
                    .spanObs{
                        font-size: 10px;
                    }
                    th{
                        text-align: center;
                    }
                </style>

                <form name=\"formulario_{$etapa}\" id=\"formulario_{$etapa}\" class=\"form-horizontal\" method=\"POST\">
                    <input type=\"hidden\" id=\"request\" name=\"request\" value=\"\"/>
                    <input type=\"hidden\" id=\"eacid\" name=\"eacid\" value=\"{$etapa}\"/>
                    <input type=\"hidden\" id=\"excluir_aceid\" name=\"excluir_aceid\" value=\"\"/>
                    <input type=\"hidden\" id=\"estuf\" name=\"estuf\" value=\"{$estuf}\"/>

                    <div id=\"listagem\" style=\"background: #ffffff !important; overflow-y: scroll;max-height: 400px\">
                        <input type=\"hidden\" id=\"contN\" name=\"contN\" value=\"{$contN}\"/>
                        <table name=\"tabAtvDes\" class=\"table table-bordered table-hover table-condensed\" border=\"0\">
                        <thead>
                            <tr>
                                
                                <th width=\"340\">Ação</th>
                                <th width=\"240\">Responsável</th>
                                <th width=\"140\">Previsão de Execução</th>
                                <th>Observações</th>
                                <th width=\"140\">Concluída em</th>
                                <th width=\"140\">Ações</th>
                            </tr>
                        </thead>
                        <tbody>
            ";

        $i = 1;
        if ($res[0]['aceid'] > 0) {
            foreach ($res as $r) {
                $r['aceacao'] = stripslashes($r['aceacao']);
                $r['aceresponsavel'] = stripslashes($r['aceresponsavel']);
                $r['aceobservacao'] = stripslashes($r['aceobservacao']);

                $html .= "
                        <tr>
                            <td>
                                <textarea readonly=\"readonly\"
                                          class=\"form-control frm-disabled{$r['aceid']} frm-obrigatorio-{$etapa}\"
                                          id=\"aceacao{$i}\"
                                          maxlength=\"1000\"
                                          name=\"aceacao[]\"
                                          rows=\"2\">{$r['aceacao']}</textarea>
                            </td>
                            <td>
                                <textarea readonly=\"readonly\"
                                          class=\"form-control frm-disabled{$r['aceid']} frm-obrigatorio-{$etapa}\"
                                          id=\"aceresponsavel{$i}\"
                                          maxlength=\"300\"
                                          name=\"aceresponsavel[]\"
                                          rows=\"2\">{$r['aceresponsavel']}</textarea>
                            </td>
                            <td>
                                <center>
                                    <input readonly=\"readonly\"
                                           id=\"aceprazo_{$etapa}_{$i}\"
                                           name=\"aceprazo_{$etapa}[]\"
                                           type=\"text\"
                                           class=\" form-control frm-disabled{$r['aceid']} frm-obrigatorio-{$etapa}\"
                                           value=\"{$r['aceprazo']}\" placeholder=\"\">
                                </center>
                            </td>
                            <td>
                                <textarea readonly=\"readonly\"
                                          class=\"form-control frm-disabled{$r['aceid']}\"
                                          id=\"aceobservacao{$i}\"
                                          maxlength=\"1000\"
                                          name=\"aceobservacao[]\"
                                          rows=\"2\">{$r['aceobservacao']}</textarea>
                            </td>
                             <td>
                                <center>
                                    <input readonly=\"readonly\"
                                           id=\"aceconcluida_{$etapa}_{$i}\"
                                           name=\"aceconcluida_{$etapa}[]\"
                                           type=\"text\"
                                           class=\" form-control frm-disabled{$r['aceid']} frm-obrigatorio-{$etapa}\"
                                           value=\"{$r['aceconcluida']}\" >
                                </center>
                            </td>
                            
";


                if (in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO, $perfis)) {

                    $r['datalimit'] = date("Y-m-d", strtotime($r['datalimit']));

                    if (strtotime(date("Y-m-d")) > strtotime($r['datalimit']) && $r['datalimit'] != '1969-12-31') {

                    } else if (date("Y") == $r['acedatainclusao']) {
                        $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['aceid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['aceid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                    <button title=\"Excluir\" class=\"btn btn-danger btnExcluir{$r['aceid']}\" type=\"button\"
                                            onclick=\"excluirAtividade({$r['aceid']})\"><span class=\"glyphicon glyphicon-trash\"></span></button>
                                                
                                    <input type=\"hidden\" name=\"aceid[]\" id=\"aceid{$i}\" value=\"{$r['aceid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                    } else if ($r['aceconcluida'] == NULL && date("Y") == $r['acedatainclusao']) {
                        $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['aceid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['aceid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                    <button title=\"Excluir\" class=\"btn btn-danger btnExcluir{$r['aceid']}\" type=\"button\"
                                            onclick=\"excluirAtividade({$r['aceid']})\"><span class=\"glyphicon glyphicon-trash\"></span></button>
                                                
                                    <input type=\"hidden\" name=\"aceid[]\" id=\"aceid{$i}\" value=\"{$r['aceid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                    }else  if ($r['aceconcluida'] == NULL && $ano_passado == $r['acedatainclusao']) {
                        $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['aceid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['aceid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                                                                   
                                    <input type=\"hidden\" name=\"aceid[]\" id=\"aceid{$i}\" value=\"{$r['aceid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                    }else  if (strtotime(date("Y-m-d")) < strtotime($r['datalimit'])) {
                        $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['aceid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['aceid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                                                                   
                                    <input type=\"hidden\" name=\"aceid[]\" id=\"aceid{$i}\" value=\"{$r['aceid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                    }


                } else if (in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis)) {

                    $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['aceid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['aceid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                    <button title=\"Excluir\" class=\"btn btn-danger btnExcluir{$r['aceid']}\" type=\"button\"
                                            onclick=\"excluirAtividade({$r['aceid']})\"><span class=\"glyphicon glyphicon-trash\"></span></button>
                                                
                                    <input type=\"hidden\" name=\"aceid[]\" id=\"aceid{$i}\" value=\"{$r['aceid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                } else {




                }


                //$select


                $html .= "<div class=\"modal fade\" id=\"edtarAtividade_{$r['aceid']}\" tabindex=\"-1\" role=\"dialog\" aria-labelledby=\"edtarAtividade_{$r['aceid']}\">
                                <div class=\"modal-dialog  modal-xl\" role=\"document\">
                                    <div class=\"modal-content\">
                                        <div class=\"modal-header\">
                                            <button type=\"button\" class=\"close\" data-dismiss=\"modal\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button>
                                            <h4 class=\"modal-title text-center\" id=\"myModalLabel\" >EDITAR DA AÇÃO DA ETAPA</h4>
                                        </div>
                                        <div class=\"modal-body\">
                                            <form name=\"formulario_edit_{$r['aceid']}\" id=\"formulario_edit_{$r['aceid']}\" class=\"form-horizontal\" method=\"POST\">
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"etapa\">Etapa:</label>
                                                    <div class=\"col-md-10\">
                                                        <select id=\"etapa_{$r['aceid']}\"   name=\"etapa\" class=\"form-control\" ".(in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis) || in_array( PFLCOD_SASE_EXECUTIVO, $perfis)? ($r['acedatainclusao'] == $ano_passado ? 'disabled' : '') : '')." >
                                                            <option value=\"0\" >Selecione</option>
                                                            <option value=\"1\" " . ($etapa == '1' ? 'selected' : '') . " > I.  ETAPA - Organizar o Trabalho</option>
                                                            <option value=\"2\" " . ($etapa == '2' ? 'selected' : '') . " > II. ETAPA - Estudar o Plano</option>
                                                            <option value=\"3\" " . ($etapa == '3' ? 'selected' : '') . " > III. ETAPA - Monitorar continuamente as metas e estratégias</option>
                                                            <option value=\"4\" " . ($etapa == '4' ? 'selected' : '') . " > IV. ETAPA - Avaliar periodicamente o plano</option>
                                                        </select>
                                                    </div>
                                                </div>
                                                <input type=\"hidden\" id=\"estuf_{$r['aceid']}\" name=\"estuf_{$r['aceid']}\" value=\"{$_GET['estuf']}\"/>
                                                <input type=\"hidden\" id=\"aceid_{$r['aceid']}\" name=\"aceid_{$r['aceid']}\" value=\"{$r['aceid']}\"/>
                                                
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"aceacao\">Ação:</label>
                                                    <div class=\"col-md-10\">
                                                        <textarea class=\"form-control\" ".(in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO, $perfis) ? ($r['acedatainclusao'] == $ano_passado ? 'disabled' : '') : '')." cols=\"40\" id=\"aceacao_{$r['aceid']}\"  name=\"aceacao_{$r['aceid']}\">{$r['aceacao']}</textarea>
                                                    </div>
                                                </div>
                                                
                            
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"aceresponsavel\">Responsável:</label>
                                                    <div class=\"col-md-10\">
                                                        <textarea class=\"form-control\" ".(in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO, $perfis) ? ($r['acedatainclusao'] == $ano_passado ? 'disabled' : '') : '')." id=\"aceresponsavel_{$r['aceid']}\" name=\"aceresponsavel_{$r['aceid']}\">{$r['aceresponsavel']}</textarea>
                                                    </div>
                                                </div>
                            
                            
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"aceprazo\">Previsão de Execução:</label>
                                                    <div class=\"col-md-10\">
                                                        <input id=\"aceprazo_{$r['aceid']}\" ".(in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO, $perfis) ? ($r['acedatainclusao'] == $ano_passado ? 'disabled' : '') : '')."  maxlength=\"0\" name=\"aceprazo_{$r['aceid']}\" width=\"200px\" type=\"text\" value=\"{$r['aceprazo']}\" class=\"form-control  campoData2 input-md\">
                            
                                                    </div>
                                                </div>
                            
                            
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"aceobservacao\">Observações:</label>
                                                    <div class=\"col-md-10\">
                                                        <textarea class=\"form-control\" ".(in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO, $perfis) ? ($r['acedatainclusao'] == $ano_passado ? 'disabled' : '') : '')." id=\"aceobservacao_{$r['aceid']}\" name=\"aceobservacao_{$r['aceid']}\">{$r['aceobservacao']}</textarea>
                                                    </div>
                                                </div>
                                                
                                                <div class=\"form-group\">
                                                           <label class=\"col-md-2 control-label\" for=\"aceconcluida\">Concluída em:</label>
                                                           <div class=\"col-md-10\">
                                                            <input id=\"aceconcluida_{$r['aceid']}\" maxlength=\"0\" name=\"aceconcluida_{$r['aceid']}\" width=\"200px\" type=\"text\" value=\"{$r['aceconcluida']}\" class=\"form-control  campoData2 input-md\">
                                                    </div>
                                                </div>
                            
                            
                                        </div>
                                        <div class=\"modal-footer\">
                                            <button type=\"button\" class=\"btn btn-default\"  data-dismiss=\"modal\">Cancelar</button>
                                            <button  type=\"button\"  class=\"btn btn-primary\" onclick=\"editarEtapa({$r['aceid']})\"><span class=\"glyphicon glyphicon-plus\"></span> Salvar</button>
                                        </div>
                                        </form>
                                    </div>
                                </div>
                            </div>
                    </td>
                        </tr>
                    ";
                $i++;
            }

        } else {
            $html .= "
                    <tr>
                        <td  colspan='6' style=\"vertical-align: middle;\">
                            <div class=\"alert alert-info text-center\">
                           Nenhum registro encontrado!
                        </div>
                        </td>
                    </tr>
                    
                ";
        }

        $html .= "
                    </tbody>
                        
                    </table>
                </div>
            </form>
            
            
            
            
            ";
        return $html;
    }

    //botao historico
    public function dadosHistorico($estuf, $eacid)
    {
        global $db;
        $sql = <<<DML
            
            
SELECT
                    est.estuf,
                    hace.haceacao,
                    hace.haceresponsavel,
                    to_char(hace.haceprazo, 'DD/MM/YYYY') as aceprazo,
                    hace.haceobservacao,
                     to_char(hace.haceconcluida, 'DD/MM/YYYY') as haceconcluida,
                    usu.usucpf,
                    usu.usunome,
                    to_char(ace.acedatainclusao, 'DD/MM/YYYY HH:MI:SS') as acedatainclusao,
                    to_char(hace.hacedatainclusao, 'DD/MM/YYYY HH:MI:SS') as acedataalteracao,
                    
                     hace.hacemodificacao

                FROM sase.historicoagendacomissaocoordestado hace
                inner join territorios.estado est on hace.estuf = est.estuf
                inner join seguranca.usuario usu on hace.usucpf = usu.usucpf
                left join sase.agendacomissaocoordestado ace on ace.aceid = hace.aceid
                WHERE  hace.estuf = '{$estuf}' AND ace.eacid = {$eacid}
                ORDER BY ace.acedataalteracao desc
            
                
DML;


        $cabecalho = array("Estado", "Ação", "Responsável", "Previsão de Execução", "Observação", "Concluída em", "CPF", "Usuário", "Data de Inclusão", "Data de Alteração/Exclusão", "Operação");
        $alinhamento = array("left", "left", "left", "left", "left", "left", "left", "left");
        $largura = array();
        $historico = $db->monta_lista($sql, $cabecalho, 30, 5, 'N', '', 'N', 'historico', $largura, $alinhamento);
        // -- popup listagem histórico

        return $historico;
    }

    //salvar por etapa
    public function salvarAgendaEstado($dados)
    {

        $eacid = $dados['etapa'];
        $estuf = $dados['estuf'];
        $aceacao = $dados['aceacao'];
        $aceresponsavel = $dados['aceresponsavel'];
        $aceprazo = formata_data_sql($dados['aceprazo']);
        $aceobservacao = $dados['aceobservacao'];
        $usucpf = $_SESSION['usucpf'];

        $_sql_1 = "INSERT INTO sase.agendacomissaocoordestado(
                                eacid,
                                estuf,
                                aceacao,
                                aceresponsavel,
                                aceprazo,
                                aceobservacao,
                                usucpf,
                                acedatainclusao
                            )VALUES(
                                {$eacid}, '{$estuf}', '{$aceacao}', '{$aceresponsavel}', '{$aceprazo}', '{$aceobservacao}', '{$usucpf}', 'NOW()');                        
                    ";

        $this->executar($_sql_1);
        $this->commit();

        $sql = "SELECT max(aceid) from sase.agendacomissaocoordestado";
        $aceidinserido = $this->pegaum($sql);
        $_sql_2 = "
                        INSERT INTO sase.historicoagendacomissaocoordestado(
                                eacid,
                                estuf,
                                haceacao,
                                haceresponsavel,
                                haceprazo,
                                haceobservacao,
                                usucpf,
                                hacedatainclusao,
                                hacestatus,
                                aceid,
                                hacemodificacao
                                
                            )VALUES(
                                {$eacid}, '{$estuf}', '{$aceacao}', '{$aceresponsavel}', '{$aceprazo}', '{$aceobservacao}', '{$usucpf}', 'NOW()', 'A' , {$aceidinserido} , 'Inclusão' );
                        ";

        $this->executar($_sql_2);


        $isTrue = $this->commit();

        $_SESSION['msg'] = "Inserido com sucesso!";


        return $isTrue;
    }

    //salvar por etapa
    public function editarAgendaEstado($dados)
    {


        //header("Content-Type: text/html; charset=ISO-8859-1",true) ;
        $dados = array_map("utf8_decode", $dados);

        $eacid = $dados['eacid'];
        $aceid = $dados['aceid'];
        $estuf = $dados['estuf'];
        $aceacao = $dados['aceacao'];
        $aceresponsavel = $dados['aceresponsavel'];
        $aceprazo = formata_data_sql($dados['aceprazo']);
        $aceobservacao = $dados['aceobservacao'];
        $usucpf = $_SESSION['usucpf'];
        $aceconcluida = formata_data_sql($dados['aceconcluida']);

        if ($aceconcluida != '') {
            $insertaceconcluida = " and  aceconcluida  = '{$aceconcluida}'";
        } else {
            $insertaceconcluida = "and aceconcluida      = null";
        }

        $sql = "SELECT  aceid FROM sase.agendacomissaocoordestado 
                      WHERE aceid = {$aceid} AND 
                            estuf  = '{$estuf}' AND 
                            aceacao  = '{$aceacao}' AND 
                            aceresponsavel  = '{$aceresponsavel}' AND 
                            aceprazo  = '{$aceprazo}' AND
                            acestatus = 'A'  {$insertaceconcluida}
 ";
        $resposnta = $this->pegaum($sql);

        if ($resposnta > 0) {
            $_SESSION['msg'] = "Altere ao menus um campo!";
            return;
        }

        if ($aceconcluida != '') {

            $upaceconcluida = ", aceconcluida      = '{$aceconcluida}'";
            $aceconcluida = "'{$aceconcluida}'";
        } else {
            $upaceconcluida = ", aceconcluida      = NULL";
            $aceconcluida = "NULL";
        }

        $_sql_1 = "

            UPDATE  sase.agendacomissaocoordestado
                        SET
                          eacid             =  {$eacid},
                          estuf             = '{$estuf}',
                          aceacao           = '{$aceacao}',
                          aceresponsavel    = '{$aceresponsavel}',
                          aceprazo          = '{$aceprazo}',
                          aceobservacao     = '{$aceobservacao}',
                          acestatus         = 'A',
                          usucpf            = '{$usucpf}',
                          acedataalteracao  = 'NOW()'
                         {$upaceconcluida}
                        WHERE aceid = {$aceid};


                INSERT INTO sase.historicoagendacomissaocoordestado(
                        eacid, estuf, haceacao, haceresponsavel, haceprazo, haceobservacao, usucpf, hacedatainclusao, aceid ,hacemodificacao , haceconcluida 
                    )VALUES(
                        {$eacid}, '{$estuf}', '{$aceacao}', '{$aceresponsavel}', '{$aceprazo}', '{$aceobservacao}', '{$usucpf}', 'NOW()',{$aceid}, 'Alteração' , {$aceconcluida}
                );

            ";

        $this->executar($_sql_1);
        $this->commit();


        $_SESSION['msg'] = "Alterado com sucesso!";
        return true;
    }

    //legenda da aba mapa
    public function montaLegendaEstado(Array $estuf)
    {
        global $db;
        $where = (($estuf != '' && count($estuf) > 0) ? " and e.estuf in ( '" . (implode("','", $estuf)) . "' ) " : "");

        $sql = <<<DML
                select
                    (SELECT
                            count(DISTINCT e.*)
                        FROM territorios.estado AS e
                        LEFT JOIN sase.agendacomissaocoordestado AS a ON a.estuf = e.estuf
                        WHERE 1=1
                            {$where}
                        AND acedatainclusao IS NOT NULL
                        AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado acc where eacid  = 1 and acestatus = 'A' AND acc.estuf = e.estuf AND ( aceacao != '' AND aceresponsavel != '' AND aceprazo IS NOT NULL) ) )
                        --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 1 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                        --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 2 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                        --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 3 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                        --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 4 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )               
                    ) as preenchido,
                    (SELECT 
                            count(DISTINCT e.*)
                         FROM territorios.estado AS e
                         LEFT JOIN sase.agendacomissaocoordestado AS a ON a.estuf = e.estuf
                         WHERE 1=1
                            {$where}
                         AND 
                             acedatainclusao IS NULL
                    ) as naopreenchido
DML;
        $res = $db->pegaLinha($sql);

        $html = <<<HTML
                <div id="legendaMapaContainer">
                <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
                <ul>
                    <li>
                        <table>
                            <tr>
                                <td>
                                    <span style='background:#00cc00;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                    <b>{$res['preenchido']}</b>&nbsp;&nbsp;
                                </td>
                                <td>Preenchido</td>
                            </tr>
                            <tr><td></td></tr>
                            <tr>
                                <td>
                                    <span style='background:#cc0000;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                    <b>{$res['naopreenchido']}</b>&nbsp;&nbsp;
                                </td>
                                <td>Não Preenchido</td>
                            </tr>
                        </table>
                    </li>
                </ul>
HTML;
        echo $html;
    }

    /**
     * Função para montar arquivo excel - Estados
     * @access public
     * @param array $_POST
     * @return array
     */
    public function montaListaXls($dados)
    {
        global $db;

        if ($dados['estuf'] != '') {
            $and .= " AND e.estuf = '{$dados['estuf']}'";
        }

        if ($dados['preenchido'] == 'S') {
            $and .= "AND acedatainclusao IS NOT NULL
                     AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado acc where eacid  = 1 and acestatus = 'A' AND acc.estuf = e.estuf AND ( aceacao != '' AND aceresponsavel != '' AND aceprazo IS NOT NULL) ) )
                     --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 1 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                     --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 2 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                     --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 3 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                     --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 4 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                    ";
        } elseif ($dados['preenchido'] == 'N') {
            $and .= "AND acedatainclusao IS NULL";
        }

        $SQL = "
           SELECT  DISTINCT 
                    e.estuf||' - '||estdescricao as estado,
                    CASE WHEN acedatainclusao IS NOT NULL 
                            AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado acc where eacid  = 1 and acestatus = 'A' AND acc.estuf = e.estuf AND ( aceacao != '' AND aceresponsavel != '' AND aceprazo IS NOT NULL) ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 1 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 2 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 3 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 4 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                        THEN 'S'
                        ELSE 'N'
                    END AS preenchido
            FROM  sase.agendacomissaocoordestado AS acee
            RIGHT JOIN territorios.estado as e ON acee.estuf = e.estuf 
            WHERE 1 = 1
            {$and}
            ORDER BY 2, 1
        ";

        $cabecalho = array('Estado', 'Agenda Preenchida');
        $alinhamento = array('left', 'left', 'left', "left", 'left', 'left');
        $larguras = array('5%', '5%', '75%', '10%', '5%', '5%');

        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=ssimec_sase_agenda_trabalho_" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=simec_sase_agenda_trabalho_" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");

        $db->monta_lista($SQL, $cabecalho, 100000, 5, 'N', '', 'N', 'listaAgendaTrabalho', $larguras, $alinhamento);
    }

    //legenda mapa externo
    public function montaLegendaEstadoExterno()
    {
        global $db; ?>
        <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
            <div id="tituloLegenda">
                <div style="float:left;"><h5
                            style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5>
                </div>
                <div style="float:right;"></div>
                <div style="clear:both;height:1px;">&nbsp;</div>
            </div>
            <ul>
                <?php
                require_once dirname(str_replace('/Model', '', (str_replace('\Model', '', __FILE__)))) . '/Mapa/Metadados/DadoAgendaEstadoComissaoCoordenadoraExterno.php';
                $classeDados = 'DadoAgendaEstadoComissaoCoordenadoraExterno';
                $legenda = (new $classeDados())->dadosDaLegenda();
                foreach ($legenda as $value) {
                    echo <<<HTML
                        <li>
                            <table>
                                <tr>
                                    <td style="width:47px;">
                                        <span style="background:{$value['cor']};" class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$value['total']}</b>&nbsp;&nbsp;
                                    </td>
                                    <td>{$value['descricao']}</td>
                                </tr>
                            </table>
                        </li>
HTML;
                }
                ?>
            </ul>
        </div>
        <?php
    }

    public function buscaEstado($estuf)
    {
        $sql = "select estuf || ' - ' || estdescricao as estado
                 from territorios.estado 
                 where estuf = '{$estuf}'";

        return $this->pegaUm($sql);
    }

    public function excluirItemAgendaTrabalho($aceid)
    {
        $sql = "UPDATE sase.agendacomissaocoordestado
                SET acestatus = 'I', acedataalteracao= 'NOW()'
                WHERE aceid = {$aceid}";

        $this->executar($sql);
        $this->commit();
    }
}
