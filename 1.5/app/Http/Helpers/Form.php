<?php

namespace App\Http\Helpers;

use Collective\Html\FormFacade;
use Carbon\Carbon as Carbon;

/**
 * @see \Collective\Html\FormBuilder
 */
class Form extends FormFacade {

    /**
     * Get the registered name of the component.
     *
     * @return string
     */
    protected static function getFacadeAccessor() {
        return "form";
    }

    public static function text2($nome, $model, $errors = null, $options = null, $labelMedidas = [2, 10]) {

        $divLabel = null;
        self::addClassOption($options);

        if (array_key_exists('mask', $options)) {
            $options['onkeyup'] = "this.value=mascaraglobal('" . $options['mask'] . "',this.value);";
            $options['onblur'] = "this.value=mascaraglobal('" . $options['mask'] . "',this.value);";
            unset($options['mask']);
        }

        self::adicionarValidate($nome, $model, $options);
        self::adicionarPlaceholder($nome, $model, $options);

        $divLabel = self::adicionarLabel($nome, $model, $labelMedidas[0], $options);
        $input = parent::text($nome, $model->$nome, $options);

        $divInput = self::montarDivsInput($input, $nome, $errors, $labelMedidas[1]);

        $html = self::montarDivs($nome, $divLabel, $divInput, $errors);


        return new \Illuminate\Support\HtmlString(preg_replace("/(\v|\s)+/", " ", $html));
    }

    public static function number2($nome, $model, $errors = null, $options = null, $labelMedidas = [2, 10]) {

        $divLabel = null;
        self::addClassOption($options);

        if (array_key_exists('mask', $options)) {
            $options['onkeyup'] = "this.value=mascaraglobal('" . $options['mask'] . "',this.value);";
            $options['onblur'] = "this.value=mascaraglobal('" . $options['mask'] . "',this.value);";
            unset($options['mask']);
        }

        self::adicionarValidate($nome, $model, $options);
        self::adicionarPlaceholder($nome, $model, $options);

        $divLabel = self::adicionarLabel($nome, $model, $labelMedidas[0], $options);
        $input = parent::number($nome, $model->$nome, $options);

        $divInput = self::montarDivsInput($input, $nome, $errors, $labelMedidas[1]);

        $html = self::montarDivs($nome, $divLabel, $divInput, $errors);


        return new \Illuminate\Support\HtmlString(preg_replace("/(\v|\s)+/", " ", $html));
    }

    public static function select2($nome, $model, $list = [], $errors = null, $selected = null, $options = [], $labelMedidas = [2, 10]) {
        if (!$options)
            $options = [];
        $divLabel = null;

        self::addClassOption($options);

        if (empty($options["placeholder"])) {
            $options["placeholder"] = ".. Selecione ..";
        }

        self::adicionarValidate($nome, $model, $options);

        $input = parent::select($nome, $list, $selected, $options);


        $divLabel = self::adicionarLabel($nome, $model, $labelMedidas[0], $options);

        $divInput = self::montarDivsInput($input, $nome, $errors, $labelMedidas[1]);

        $html = self::montarDivs($nome, $divLabel, $divInput, $errors);

        return new \Illuminate\Support\HtmlString(preg_replace("/(\v|\s)+/", " ", $html));
    }

    public static function textarea2($nome, $model, $errors = null, $options = [], $labelMedidas = [2, 10]) {
        $divLabel = null;

        self::addClassOption($options);

        self::adicionarValidate($nome, $model, $options);


        $divLabel = self::adicionarLabel($nome, $model, $labelMedidas[0], $options);
        $input = parent::textarea($nome, $model->$nome, $options);

        $divInput = self::montarDivsInput($input, $nome, $errors, $labelMedidas[1]);

        $html = self::montarDivs($nome, $divLabel, $divInput, $errors);


        return new \Illuminate\Support\HtmlString(preg_replace("/(\v|\s)+/", " ", $html));
    }

    public static function radioGroup2($nome, $model, $valores, $errors = null, $options = [], $labelMedidas = [2, 10]) {
        $divLabel = null;

        self::adicionarValidate($nome, $model, $options);


        $divLabel = self::adicionarLabel($nome, $model, $labelMedidas[0], $options);

        $radios = null;
        
        foreach ($valores as $key => $value) {
            $check = ($model->$nome == $key) ? true : false;
            $options['id'] = $nome . '_' . $key;
            $radio = parent::radio($nome, $key, $check, $options)->__toString();
            $horizontal = null;
      
            
            if ($options && in_array("horizontal", $options)) {
                $horizontal = 'style="float:left;padding-right: 10px"';
            }

            $radios[] = '<div class="radio radio-primary" ' . $horizontal . ' ><label>' . $radio . $value . '</label></div>';
        }

        $radios = implode("", $radios);

        $divInput = self::montarDivsOption($radios, $nome, $errors, $labelMedidas[1], true);

        $html = self::montarDivs($nome, $divLabel, $divInput, $errors);

        return new \Illuminate\Support\HtmlString(preg_replace("/(\v|\s)+/", " ", $html));
    }

    public static function file2($nome, $model, $errors = null, $options = null, $labelMedidas = [2, 10]) {
        $divLabel = null;

        self::addClassOption($options);

        self::adicionarValidate($nome, $model, $options);

        $divLabel = self::adicionarLabel($nome, $model, $labelMedidas[0], $options);

        $input = parent::file($model->$nome, $options);

        $divInput = self::montarDivsInput($input, $nome, $errors, $labelMedidas[1]);

        $html = self::montarDivs($nome, $divLabel, $divInput, $errors);


        return new \Illuminate\Support\HtmlString(preg_replace("/(\v|\s)+/", " ", $html));
    }

    public static function date2($nome, $model, $errors = null, $options = null, $labelMedidas = [2, 10]) {
        $divLabel = null;

        $options['class'] = (!empty($options['class'])) ? $options['class'] . ' datepicker' : ' datepicker';
        self::addClassOption($options);

        self::adicionarValidate($nome, $model, $options);

        $divLabel = self::adicionarLabel($nome, $model, $labelMedidas[0], $options);

        $modelNome = $model->$nome;

        if ($modelNome instanceof Carbon) {
            $modelNome = Carbon::parse($modelNome)->format('d/m/Y');
        }

        $input = parent::text($nome, $modelNome, $options);

        $divInput = self::montarDivsInputDate($input, $nome, $errors, $labelMedidas[1]);

        $html = self::montarDivs($nome, $divLabel, $divInput, $errors);

        return new \Illuminate\Support\HtmlString(preg_replace("/(\v|\s)+/", " ", $html));
    }

    public static function datePeriodo($nome1, $nome2, $model, $value = null, $errors = null, $options1 = null, $options2 = null, $labelMedidas = [2, 10]) {
        $divLabel = null;

        self::addClassOption($options);

        $divLabel = self::adicionarLabel($nome1, $model, $labelMedidas[0], $options1);

        $input1 = parent::date($nome1, $value, $options1);
        $input2 = parent::date($nome2, $value, $options2);

        $divInput1 = self::montarDivsInput($input1, $nome1, $errors, $labelMedidas[1]);
        $divInput2 = self::montarDivsInput($input2, $nome2, $errors, $labelMedidas[1]);

        $html = self::montarDivsPeriodo($nome1, $divLabel, $divInput1, $divInput2, $errors);

        return new \Illuminate\Support\HtmlString(preg_replace("/(\v|\s)+/", " ", $html));
    }

    public static function adicionarValidate($nome, $model, &$options = []) {
        if ($options && !key_exists("naovalidar", $options)) {
            $options["data-validate"] = $model->getRules($nome, true);
        }
    }

    public static function adicionarPlaceholder($nome, $model, &$options = []) {

        if ($options && !key_exists("placeholder", $options)) {

            $options["placeholder"] = $model->getLabels($nome);
        }
    }

    public static function adicionarLabel($nome, $model, $tamanho, $options = []) {

        $required = null;
        $html = null;
        $display = "none";
        if (!$options)
            $options = [];

        if (($options && array_key_exists("data-validate", $options) ) && strpos($options["data-validate"], "required") !== false)
            $required = "*";

        if ((!array_key_exists("label", $options) || (array_key_exists("label", $options) && $options["label"] === true ))) {
            $display = "block";
        }

        return <<<HTML
                    <label style="display:{$display}" class="col-md-{$tamanho} control-label text-right p-xxs">
                    {$model->getLabels($nome)}:{$required}
                    </label>
HTML;
    }

    public static function adicionarsClass($html, $model, $nome) {

        if ($html && !in_array("class", $html)) {

            $html = $html + ["class" => "form-control"];
        } else {

            $html["class"] = " form-control";
        }
        if ($html && !in_array("data-validate", $html)) {
            $html = $html + ["data-validate" => $model->getRules($nome, true)];
        }


        if ($html && !in_array("placeholder", $html)) {
            $html = $html + ["placeholder" => $model->getLabels($nome)];
        }

        return $html;
    }

    public static function montarDivsPeriodo($nome = null, $label = null, $input1 = null, $input2 = null, $errors = null) {

        $erro = ($errors->has($nome) ? "has-error" : "");

        return <<<HTML
                        <div class="form-group" {$erro} >
                        {$label}
                        {$input1}
                        <span style="float: left; display: block; margin-top: 7px">a</span>
                        {$input2}
                </div>
HTML;
    }

    public static function montarDivs($nome = null, $label = null, $input = null, $errors = null) {

        $erro = ($errors->has($nome) ? "has-error" : "");

        return <<<HTML
                        <div class="form-group" {$erro} >
                        {$label}
                        {$input}
                </div>
HTML;
    }

    public static function montarDivsInput($input, $nome, $errors, $medidas) {

        return <<<HTML
                        <div class="col-md-{$medidas}">
                         {$input}

                        <span class="text-danger">{$errors->first($nome, "")}</span>
                    </div>

HTML;
    }

    public static function montarDivsInputDate($input, $nome, $errors, $medidas) {

        return <<<HTML
                        <div class="col-md-{$medidas}" >
                        <div class="input-group date datepicker" style="padding: 0">

                            <span class="input-group-addon">
                              <i class="fa fa-calendar"></i>
                            </span>
                             {$input}
                        </div>

                        <span class="text-danger">{$errors->first($nome, "")}</span>
                    </div>

HTML;
    }

    public static function montarDivsOption($input, $nome, $errors, $labelMedidas, $class = 'form-control') {

        return <<<HTML
                        <div class="col-md-{$labelMedidas}">
                         <div class="{$class}">
                         {$input}
                        </div>
                        <span class="text-danger">{$errors->first($nome, "")}</span>
                    </div>

HTML;
    }

    public static function montarDivsLabel($model, $nome, $labelMedidas) {
        return <<<HTML
                         <label class="col-md-{$labelMedidas} control-label text-right p-xxs">
                            {$model->getLabels($nome)}
                    </label>

HTML;
    }

    public static function addClassOption(&$options) {
        if (empty($options["class"])) {
            $options["class"] = " form-control";
        } else {
            $options["class"] .= " form-control";
        }
    }

}
