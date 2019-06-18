<?php

verificaSessao('orgao');
require APPRAIZ . "obras2/includes/principal/cadObra/ctrl.php";


//$orgid = $_SESSION['obras2']['orgid'];
$docid = pegaDocidObra($obrid);

if ($obrid) {
    echo cabecalhoObra($obrid);
}

    monta_titulo_obras('Cadastro de Obra', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>


<script language="javascript" type="text/javascript"src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="formObra" id="formObra" enctype="multipart/form-data">
    <input type="hidden" name="req" value="salvar"/>
    <input type="hidden" name="obra[obridvinculado]" id="obridvinculado" value="<?php echo $obridVinculado ?>"/>
    <input type="hidden" name="obra[obrid]" id="obrid" value="<?php echo $obra->obrid ?>"/>
    <input type="hidden" name="obra[empid]"
           value="<?php echo($obra->empid ? $obra->empid : $_SESSION['obras2']['empid']) ?>"/>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <?php
        if ($obraVinculada->obrid):
            ?>
            <tr>
                <td align="justify" style="color: red;" colspan="2">
                    Você está cadastrando uma obra vinculado a obra paralisado,
                    <b><?php echo '(' . $obraVinculada->obrid . ') ' . $obraVinculada->obrdsc ?></b>, este será
                    desativado e não poderá mais sofrer alterações.
                </td>
            </tr>
            <?php
            $objChkLst = new ChecklistFnde();
            if ($objChkLst->verificaBloqueioDeVinculoViaChecklistObraVinculada($obraVinculada->obrid)) {
                $habilitado = false;
                $habilita = 'N';
                echo '<tr>
                        <td align="justify" style="color: red;" colspan="2">
                            A obra (' . $obraVinculada->obrid . ') ' . $obraVinculada->obrdsc . ' possui pendências relativas ao Checklist de Obras Vinculadas.
                            Resolva essas pendências para proseguir com a criação da Obra Vinculada.
                        </td>
                    </tr>';
            }
        endif;
        ?>
        <tr>
            <td class="SubTituloDireita" width="20%">Tipo de Ensino:</td>
            <td>
                <input type="hidden" name="orgid" value="<?php echo $_SESSION['obras2']['orgid'] ?>"/>
                <?php echo $orgao->orgdesc ?>
            </td>
            <td rowspan="8" align="right" valign="top" width="1">
                <?php
                // Barra de estado WORKFLOW
                if (possui_perfil(PFLCOD_CALL_CENTER)) {
                    wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid), array('acoes' => true));
                } else if ($docid) {
                    wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid));
                };
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Unidade Implantadora:</td>
            <td>
                <?php
                $entid_permitidas = obras_pegarUnidadesPermitidas();
                if (!$_SESSION['obras2']["obrid"]) {
                    if ($entid_permitidas) {
                        $sql = "SELECT
                                e.entid as codigo,
                                e.entnome as descricao
                            FROM
                                entidade.entidade e
                            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid
                            INNER JOIN obras2.orgaofuncao orf ON orf.funid = fue.funid
                            WHERE
                                e.entid in ( " . implode(", ", $entid_permitidas) . " ) AND orf.orgid = " . $_REQUEST["org"];
                        $db->monta_combo("obra[entid]", $sql, 'S', "Selecione...", '', '', '', '340', 'S', 'entid');
                    } else {
                        $entidade = new Entidade(($obra->entid ? $obra->entid : $dadoEmpreendimento['entidunidade']));
                        $entnome = $entidade->entnome;
                        $entid = $entidade->getPrimaryKey();
                        ?>
                        <input type="hidden" name="obra[entid]" id="entid" value="<?php echo $entid; ?>"
                               class="obrigatorio">
                        <?php
                    }
                } else {
                    $entidade = new Entidade($obra->entid);
                    $entnome = $entidade->entnome;
                    $entid = $entidade->getPrimaryKey();
                    ?>
                    <input type="hidden" name="obra[entid]" id="entid"
                           value="<?php if (isset($_SESSION['obras2']["empid"])) echo $entid; ?>" class="obrigatorio">
                    <?php
                }
                ?>
                <span id="entnome">
                <a onclick="visualizarEntidade(document.getElementById('entid').value,<?php echo $orgid ?>);"
                   style="cursor:pointer">
                    <?php echo $entnome; ?>
                </a>
                <input type="hidden" name="entnome_h" id="entnome_h" value="<?php echo $entnome; ?>">
           </span>
                <?php
                if ($habilitado && !$entid_permitidas) {
                    ?>
                    <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;"
                           onclick="inserirEntidade(document.getElementById('entid').value,<?php echo $orgid ?>);" <?php if ($habilitado == false) echo "disabled"; ?>>
                    <img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0">
                    <?php
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Secretaria:</td>
            <td>
                <?php
                $sql = Array(
                Array('codigo' => '1', 'descricao' => 'SEE'),
                Array('codigo' => '2', 'descricao' => 'FDE'),
                Array('codigo' => '3', 'descricao' => 'SDR')
                );

                $db->monta_combo('obra[entidsecretaria]', $sql, $habilita, 'Selecione...', '', '', '', 200, 'S', 'entidsecretaria', '',$obra->entidsecretaria, '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Processo:</td>
            <td>
                <?php
                echo campo_texto('obra[obrnumprocessoconv]', 'S', $habilita, '', 20, 20, '#####.######/####-##', '', 'right', '', 0, 'id="obrnumprocessoconv"', '', $obra->obrnumprocessoconv);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Convênio / Termo:</td>
            <td>
                <?php
                echo campo_texto('obra[numconvenio]', 'S', $habilita, '', 20, 20, '####################', '', 'right', '', 0, 'id="numconvenio"', '', $obra->numconvenio);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Ano:</td>
            <td>
                <?php
                echo campo_texto('obra[obranoconvenio]', 'S', $habilita, '', 4, 4, '####', '', 'right', '', 0, 'id="obranoconvenio"', '', $obra->obranoconvenio);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome da Obra:</td>
            <td><?php echo campo_texto('obra[obrnome]', 'S', $habilita, '', 70, 100, '', '', '', '', '', 'id="obrnome"', '', ($obra->obrnome ? $obra->obrnome : $dadoEmpreendimento['empdsc'])); ?></td>
        </tr>
        <?php
        if ($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL):
            ?>
            <tr>
                <td class="SubTituloDireita">Tipologia da Obra:</td>
                <td>
                    <?php
                    $sql = $tipologiaObra->listaCombo(array('orgid' => $orgid));
                    $db->monta_combo('obra[tpoid]', $sql, $habilita, 'Selecione...', '', '', '', null, 'S', 'tpoid', '', ($obra->tpoid ? $obra->tpoid : $dadoEmpreendimento['tpoid']), '');
                    ?>
                </td>
            </tr>
        <?php
        endif;
        ?>
        <tr>
            <td class="SubTituloDireita">Tipo da Obra:</td>
            <td>
                <?php
                $sql = $tipoObra->listaCombo();
                $db->monta_combo('obra[tobid]', $sql, $habilita, 'Selecione...', '', '', '', null, 'S', 'tobid', '', ($obra->tobid ? $obra->tobid : $dadoEmpreendimento['tobid']), '');
                ?>
            </td>
        </tr>
        
        <tr>
            <td class="SubTituloDireita">Classificação da Obra:</td>
            <td>
                <?php
                $sql = $classificacaoObra->listaCombo();
                $db->monta_combo('obra[cloid]', $sql, $habilita, 'Selecione...', '', '', '', null, 'S', 'cloid', '', ($obra->cloid ? $obra->cloid : $dadoEmpreendimento['cloid']), '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Descrição / Composição da Obra:</td>
            <td>
                <?php
                if ($obridVinculado) {
                    echo campo_textarea('obra[obrdsc]', 'N', $habilita, '', 100, 5, '', '', '', '', '', '', ($obra->obrdsc ? $obra->obrdsc : $dadoEmpreendimento['empcomposicao']), $param);
                } else {
                    echo campo_textarea('obra[obrdsc]', 'S', $habilita, '', 100, 5, '', '', '', '', '', '', ($obra->obrdsc ? $obra->obrdsc : $dadoEmpreendimento['empcomposicao']), $param);
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Valor Previsto (R$):</td>
            <td colspan="2"><?php echo campo_texto('obra[obrvalorprevisto]', 'S', $habilita, '', 20, 20, '[.###],##', '', 'right', '', '', 'id="obrvalorprevisto"', '', ($obra->obrvalorprevisto ? $obra->obrvalorprevisto : $dadoEmpreendimento['empvalorprevisto'])); ?></td>
        </tr>
        <?php
        if ($obraVinculada->obrid || $obra->obridvinculado):
            ?>
            <tr>
                <td class="SubTituloDireita" style="">Percentual executado aproveitável do contrato anterior (%):</td>
                <td colspan="2"><?php echo campo_texto('obra[obrperccontratoanterior]', 'S', $habilita, '', 10, 5, '[.###],##', '', 'right', '', '', 'id="obrperccontratoanterior"', '', $obra->obrperccontratoanterior); ?></td>
            </tr>
        <?php
        endif;
        ?>

        <?php if (!$_SESSION['obras2']['empid'] || $_SESSION['obras2']['orgid'] == 3) { ?>
            <tr>
                <td colspan="3">Dados da Obra</td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Esfera:</td>
                <td colspan="2">
                    <?php
                    $empreendimento->empesfera = (!empty($empreendimento->empesfera)) ? $empreendimento->empesfera : 'M';
                    $sql = Array(
                        Array('codigo' => 'M', 'descricao' => 'Municipal'),
                        Array('codigo' => 'E', 'descricao' => 'Estadual')
                    );

                    $db->monta_combo('empreendimento[empesfera]', $sql, 'N', 'Selecione...', '', '', '', 200, 'S', 'empesfera', '', $empreendimento->empesfera, '');
                    ?>
                    <input type="hidden" name="empreendimento[empesfera]" class="<?php echo $classEnd; ?>" id="empesfera"
                           value="M"
                           style="width: 5ex; padding-left: 2px") />
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Programa:</td>
                <td id="td_programa" colspan="2">
                    <?php
                    $sql = $programaFonte->listaCombo(Array('tpoid' => $_REQUEST['tpoid']));
                    $msgTxt = ($empreendimento->prfid ? 'Selecione...' : 'Selecione uma tipologia antes...');
                    $db->monta_combo('empreendimento[prfid]', $sql, $habilita, $msgTxt, '', '', '', 200, 'N', 'prfid', '', $empreendimento->prfid, '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Modalidade de Ensino:</td>
                <td colspan="2">
                    <?php
                    $sql = $modalidadeEnsino->listaCombo();
                    $db->monta_combo('empreendimento[moeid]', $sql, $habilita, 'Selecione...', '', '', '', 200, 'S', 'moeid', '', $empreendimento->moeid, '');
                    ?>
                </td>
            </tr>
        <?php } ?>
        <?php
        if ($obra->obrid):
            ?>
            <tr>
                <td colspan="3">Histórico de Paralisações</td>
            </tr>
            <tr>
                <td colspan="3">
                    <?php
                    $histParalisacao = new HistoricoParalisacao();
                    $sql = $histParalisacao->listaSql(array('obrid' => $obra->obrid));
                    $cabecalho = array('Visualizar', 'Tipo de Paralisação', 'Data da Paralisação', 'Data da Liberação', 'Responsável');
                    $db->monta_lista_simples($sql, $cabecalho, 100, 30, 'N', '100%', 'S');
                    ?>
                </td>
            </tr>
        <?php
        endif;

        $habilitadoEnd = $habilitado;
        $habilitaEnd = $habilita;
        $classEnd = 'CampoEstilo';
        if ($obrid) {
            $habilitadoEnd = false;
            $habilitaEnd = 'N';
            $classEnd = 'disabled';
        }
        ?>
        <tr>
            <td colspan="3">Local da Obra</td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label>
            </td>
            <td colspan="2">
                <input type="text" name="endereco[endcep]"
                       onkeyup="this.value = mascaraglobal('##.###-###', this.value);"
                       onblur="Entidade.__getEnderecoPeloCEP(this);" class="<?php echo $classEnd; ?>" id="endcep"
                       value="<?php echo $endereco->endcep ?>" size="13"
                       maxlength="10" <?php echo($habilitadoEnd == false ? 'disabled="disabled"' : '') ?>/>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
            </td>
        </tr>
        <tr id="escolha_logradouro_id" style="display:none">
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de
                    Logradouro:</label></td>
            <td colspan="2">
                <input type="text" name="endlog" class="<?php echo $classEnd; ?>" id="endlog"
                       value="<?php echo $endereco->endlog ?>"
                       size="48" <?php echo($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                <label id="lbLogadouro"> Logradouro: </label>
            </td>
            <td colspan="2">
                <input type="text" name="endereco[endlog]" class="<?php echo $classEnd; ?>" id="endlogradouro"
                       value="<?php echo $endereco->endlog ?>"
                       size="65" <?php echo($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label>
            </td>
            <td colspan="2">
                <input type="text" name="endereco[endnum]" class="<?php echo $classEnd; ?>" id="endnum"
                       value="<?php echo $endereco->endnum ?>" size="13"
                       maxlength="8" <?php echo($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                <label>Complemento:</label></td>
            <td colspan="2">
                <input type="text" name="endereco[endcom]" class="<?php echo $classEnd; ?>" id="endcom"
                       value="<?php echo $endereco->endcom ?>" size="65"
                       maxlength="100" <?php echo($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label>
            </td>
            <td colspan="2">
                <input type="text" name="endereco[endbai]" class="<?php echo $classEnd; ?>" id="endbai"
                       value="<?php echo $endereco->endbai ?>"
                       size="20" <?php echo($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                <label>Município: </label></td>
            <td colspan="2">
                <?php
                $municipio = new Municipio();
                $arrMunicipio = $municipio->listaComboMulti();
                $habil = $habilitadoEnd ? 'S' : 'N';
            $db->monta_combo('endereco[muncod]', $arrMunicipio, $habil, 'Selecione...', '', '', '', 200, 'S', 'muncod', '', $endereco->muncod, '');
                ?>
 
                <input type="hidden" name="endereco[estuf]" class="<?php echo $classEnd; ?>" id="estuf"
                       value="SP"
                       style="width: 5ex; padding-left: 2px") />
            </td>
        </tr>
        <tr>
            <td colspan="3">Coordenadas Geográficas</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Latitude</td>
            <td colspan="2">
                <?php
                $medlatitude = $endereco->medlatitude;
                $latitude = explode(".", $medlatitude);
                $graulatitude = $latitude[0];
                $minlatitude = $latitude[1];
                $seglatitude = $latitude[2];
                $pololatitude = $latitude[3];
                ?>
                <?php echo campo_texto('graulatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?>
                °
                <?php echo campo_texto('minlatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?>
                '
                <?php echo campo_texto('seglatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?>
                ''
                <?php
                if (trim($pololatitude) == "S") {
                    echo "&nbsp;<span id=pololatitude_>S</span>";
                } elseif (trim($pololatitude) == "N") {
                    echo "&nbsp;<span id=pololatitude_>N</span>";
                } else {
                    echo "&nbsp;<span id=pololatitude_></span>";
                }
                ?>
                <input type="hidden" name="pololatitude" id="pololatitude" value="<?php echo trim($pololatitude) ?>">
                <?php
                if (empty($obridVinculado)) {
                    print obrigatorio();
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Longitude</td>
            <td colspan="2">
                <?php
                $medlongitude = $endereco->medlongitude;
                $longitude = explode(".", $medlongitude);
                $graulongitude = $longitude[0];
                $minlongitude = $longitude[1];
                $seglongitude = $longitude[2];
                $pololongitude = $longitude[3];
                ?>
                <?php echo campo_texto('graulongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?>
                °
                <?php echo campo_texto('minlongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?>
                '
                <?php echo campo_texto('seglongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"'); ?>
                ''
                <?php
                if (trim($pololongitude) == "W") {
                    echo "&nbsp;<span id=pololatitude_>W</span>";
                } elseif (trim($pololongitude) == "E") {
                    echo "&nbsp;<span id=pololatitude_>E</span>";
                } else {
                    echo "&nbsp;<span id=pololatitude_></span>";
                }
                ?>
                <input type="hidden" name="pololatitude" id="pololatitude" value="<?php echo trim($pololongitude) ?>">
                <?php
                if (empty($obridVinculado)) {
                    print obrigatorio();
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"></td>
            <td colspan="2">
                <?php
                if ($habilitadoEnd):
                    ?>
                    <a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a>
                <?php
                elseif (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_GESTOR_MEC)) && !$obra->preid):
                    ?>
                    <a href="#" onclick="abreEdicaoEndereco();">Editar Endereço da Obra</a>
                <?php
                endif;
                ?>
                <input style="display: none;"
                       type="text"
                       name="endereco[endzoom]"
                       id="endzoom"
                       value=<?php if ($endereco->endzoom == null) echo "15"; else echo $endereco->endzoom; ?>>
            </td>
        </tr>


        <?php

        if ($endereco->medlongitude) { ?>
            <tr>
                <td colspan="3">Localização geográfica</td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Mapa</td>
                <td colspan="2">

                    <button id="map-canvas" class='btn btn-default btn-sm'><span class='glyphicon glyphicon-map-marker'
                                                                                 aria-hidden='true'></span> Abrir Mapa
                    </button>

                </td>
            </tr>
        <?php } ?>

        <tr>
            <td colspan="3" class="divLabelForm" style="text-align: center"><b>Contatos</b></td>
        </tr>
        <tr>
            <td colspan="3">
                <?php
                $contato = new Contato();
                $_POST['estuf'] = $municipio->estuf;
                $_POST['orgid'] = $orgid;
                $sql = $contato->listaResumoSqlSemCPF($_POST);
                $cabecalho = array("Nome", "E-mail", "Telefone");

                $db->monta_lista_simples($sql, $cabecalho, 30, 5, 'N');
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="3" class="divLabelForm" style="text-align: center"><b>Responsáveis pela Obra</b></td>
        </tr>
        <tr>
            <td colspan="3">
                <?php
                $contato = new Contato();
                $sql = $contato->ReponsavelObraEGestorUnidade($empid);
                $cabecalho = array("Nome", "E-mail", "Telefone", "Perfil", 'Situação do cadastro');
                //				$db->monta_lista($sql,$cabecalho,30,5,'N','center','');
                $db->monta_lista_simples($sql, $cabecalho, 30, 5, 'N');
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="3" class="divLabelForm" style="text-align: center"><b>Contatos do PAR</b></td>
        </tr>
        <tr>
            <td colspan="3"><a href="#" onclick="abrirContatosPar(<?php echo $_SESSION['obras2']['obrid'] ?>);"><img
                            style="cursor:pointer;" src="/imagens/consultar.gif" border="0"
                            title="Visualizar Contatos PAR"> Visualizar Contatos PAR</a></td>
        </tr>
        <?php
        if (habilitaDesbloqueio($empreendimento->empesfera) && (possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO))):
            ?>
            <tr>
                <td colspan="3" class="divLabelForm" style="text-align: center"><b>Desbloqueio</b></td>
            </tr>
            <tr>
                <td colspan="3">
                    <a href="#" onclick="desbloqueioObras(<?php echo $_SESSION['obras2']['obrid'] ?>);">
                        <img style="cursor:pointer;" src="/imagens/consultar.gif" border="0"
                             title="Desbloqueio de Obras"> Desbloqueio de Obras
                    </a>
                </td>
            </tr>
        <?php
        endif;
        ?>
        <tr>
            <td class="divTituloPadrao" style="text-align: center" colspan="3">
            <?php if ($habilitado): ?>
                    <input type="button" name="salvar" class="salvar" value="Salvar"/>
            <?php endif; ?>
            </td>
        </tr>
    </table>
</form>

<?php
$objObras = new Obras();
$objObras->carregarPorIdCache($obrid);
$blockEdicao = $objObras->verificaObraVinculada();
if ($blockEdicao) {
    echo '<script type="text/javascript">';
    echo " setTimeout(bloqueiaForm('formObra'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                   }
                 ";
    echo '</script>';
}

require APPRAIZ . "obras2/includes/principal/cadObra/js.php";

?>

